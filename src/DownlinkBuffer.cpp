#include "hw/DownlinkBuffer.hpp"
#include "constants.hpp"
#include "common/SanlaProcessor.hpp"
#include "common/utils.hpp"

namespace sanla {
    namespace hw_interfaces {
        namespace mq {

            void DownlinkBuffer::ReceivePacket(SanlaPacket &packet) {
                StorePacket(packet);
            }

            uint32_t DownlinkBuffer::GetBufferLength() {
                return downlinkPacketBuffer.size();
            }

            bool DownlinkBuffer::StorePacket(SanlaPacket &packet) {
                if (downlinkPacketBuffer.size() >= DOWNLINKBUFFER_MAX_SIZE) {
                    // TODO add debug line here.
                    (void)packet;
                    return false;
                }

                // Try to find existing record of DownlinkPacket and push payload if found.
                try {
                    auto *dl_packet = downlinkPacketBuffer.at(packet.header.message_id);
                    std::string body_string(packet.body);
                    dl_packet->payloadBuffer.push_back(body_string);

                    // Validate if DownlinkPacket is ready to be turned into SanlaMessage.
                    if (validateMessageReady(*downlinkPacketBuffer[packet.header.message_id])) {
                        //Send it to store and remove downlinkPacketBuffer element
                        
                        SanlaPackage message = messaging::downlinkpacketToSanlamessage(*dl_packet);
                        SendMessageToStore(message);
                        downlinkPacketBuffer.erase(packet.header.message_id);
                        dl_packet = nullptr;
                            
                    }
                } catch(const std::out_of_range& e) {
                    DownlinkPacket dl_packet;
                    dl_packet = messaging::sanlapacketToDownlinkpacket(packet);
                    downlinkPacketBuffer[packet.header.message_id] = &dl_packet;
                }
                // packet is either stored to downlinkPacketBuffer either way at this point
                // we can drop the packet here
                (void)packet;
                return true;
            }

            void DownlinkBuffer::SendMessageToStore(SanlaPackage &message){
                if (m_sanla_processor_ptr != nullptr) {
                    auto processor = static_cast<SanlaProcessor*>(m_sanla_processor_ptr);
                    processor->StoreCompleteMessage(message);
                }
                else {
                    // Throw some error here
                }
            }

            bool validateMessageReady(DownlinkPacket downlinkPacket) {
                std::string downlinkPayload;
                for (auto const& str : downlinkPacket.payloadBuffer) {
                    downlinkPayload += str;
                }
                
                if (downlinkPayload.length() == downlinkPacket.message_payload_length) {
                    /*
                    TODO deferred scope.
                    If length matches, calculate checksum to verify integrity of the message.
                    */
                    return true;
                }
                return false;
            }
        }
    }
}