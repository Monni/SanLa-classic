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
                    // TODO add debug line here.
                    DropPacket(packet);
                    return false;
                }

                try {
                    auto *downlinkPacket = downlinkPacketBuffer.at(packet.header.message_id);
                    std::string body_string(packet.body);
                    downlinkPacket->payloadBuffer.push_back(body_string);
                } catch(const std::out_of_range& e) {
                    DownlinkPacket downlinkPacket(packet);
                    downlinkPacketBuffer[packet.header.message_id] = &downlinkPacket;
                }

                if (validateMessageReady(*downlinkPacketBuffer[packet.header.message_id])) {
                    // Todo send to MessageStore and remove 
                }

                return true;
            }

            bool validateMessageReady(DownlinkPacket downlinkPacket) {
                std::string downlinkPayload;
                for (auto const& str : downlinkPacket.payloadBuffer) {
                    downlinkPayload += str;
                }
                
                if (downlinkPayload.length() == downlinkPacket.message_payload_length) {
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