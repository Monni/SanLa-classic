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

                try {
                    auto *downlinkPacket = downlinkPacketBuffer.at(packet.header.message_id);
                    std::string body_string(packet.body);
                    downlinkPacket->payloadBuffer.push_back(body_string);
                    if (validateMessageReady(*downlinkPacketBuffer[packet.header.message_id])) {
                        // Message is complete. Send it to store and remove downlinkPacketBuffer element
                        // call function to create a SanlaPackage from downlinkPacket
                        // if(SendMessageToStore(message)){
                        //     (void)&downlinkPacket;
                        // }
                        // else {
                        //     return false;
                        // }
                    }
                } catch(const std::out_of_range& e) {
                    DownlinkPacket dl_packet;
                    dl_packet = messaging::sanlapacket_to_downlinkpacket(packet);
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
                    return true; // TODO if length matches, calculate checksum to verify integrity of the message.
                }
                return false;
            }
        }
    }
}