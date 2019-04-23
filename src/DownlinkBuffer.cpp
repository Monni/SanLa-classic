#include "hw/DownlinkBuffer.hpp"
#include "constants.hpp"

namespace sanla {
    namespace hw_interfaces {
        namespace mq {

            void DownlinkBuffer::onPacketReceived(SanlaPacket packet) {
                auto result = parser.ParseMessage(packet);
                switch (result)
                {
                    case sanlamessage::STORE:
                        StorePacket(packet);
                        break;
                    
                    case sanlamessage::DROP:
                        DropPacket(packet);
                        break;
                    
                    case sanlamessage::RESPOND:
                        RespondPacket(packet);
                        break;
                
                    default:
                        break;
                }
            }

            uint32_t DownlinkBuffer::GetBufferLength() {
                return downlinkPacketBuffer.size();
            }

            void DownlinkBuffer::DropPacket(SanlaPacket packet){
                (void)packet;
            }

            bool DownlinkBuffer::StorePacket(SanlaPacket packet) {
                if (downlinkPacketBuffer.size() >= DOWNLINKBUFFER_MAX_SIZE) {
                    DropPacket(packet);
                    return false;
                }

                try {
                    auto *downlinkPacket = downlinkPacketBuffer.at(packet.header.message_id);
                    std::string body_string(packet.body);
                    downlinkPacket->payloadBuffer.push_back(body_string);
                } catch(const std::out_of_range& e) {
                    DownlinkPacket downlinkPacket = PacketToDownlinkPacket(packet);
                    downlinkPacketBuffer[packet.header.message_id] = &downlinkPacket;
                }

                if (validateMessageReady(*downlinkPacketBuffer[packet.header.message_id])) {
                    // Todo send to MessageStore and remove 
                }

                return true;
            }

            DownlinkPacket PacketToDownlinkPacket(SanlaPacket packet) {
                DownlinkPacket downlinkPacket;

                downlinkPacket.message_id = packet.header.message_id;
                memcpy(&downlinkPacket.recipient_id, packet.header.recipient_id, sanla::sanlamessage::RECIPIENT_ID_MAX_SIZE);
                downlinkPacket.message_payload_length = packet.header.message_payload_length;
                downlinkPacket.payload_chks = packet.header.payload_chks;                
                std::string body_string(packet.body);
                downlinkPacket.payloadBuffer.push_back(body_string);

                return downlinkPacket;
            }

            bool validateMessageReady(DownlinkPacket downLinkPacket) {
                std::string downlinkPayload;
                for (std::vector<std::string>::iterator it = downLinkPacket.payloadBuffer.begin() ; it != downLinkPacket.payloadBuffer.end(); ++it)
                    downlinkPayload += *it;
                
                if (downlinkPayload.length() == downLinkPacket.message_payload_length) {
                    return true; // TODO if length matches, calculate checksum to verify integrity of the message.
                }
                return false;
            }

            void DownlinkBuffer::RespondPacket(SanlaPacket packet) {
                //TODO formulate response and push it to object which manages both uplink and downlink buffers
            }
        }
    }
}