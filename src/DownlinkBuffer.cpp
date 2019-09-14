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
                Serial.println("DownlinkBuffer::StorePacket");
                if (downlinkPacketBuffer.size() >= DOWNLINKBUFFER_MAX_SIZE) {
                    // TODO add debug line here in case dl_buffer is full.
                    (void)packet;
                    return false;
                }

                // Validate incoming packet.
                validatePacket(packet);

                // Try to find existing record of DownlinkPacket and push payload if found.
                try {
                    auto *dl_packet = downlinkPacketBuffer.at(packet.header.message_id);
                    std::string body_string(packet.body);

                    // Push payload to Map.
                    dl_packet->payloadBuffer[packet.header.payload_seq] = body_string;

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
                    processor->HandleMessage(message);
                }
                else {
                    // Throw some error here
                }
            }

            bool DownlinkBuffer::validatePacket(SanlaPacket &packet) {
                /*
                TODO
                Validate checksum
                */

                if (packet.header.payload_length == strlen(packet.body)) {
                    return true;
                }
                return true;
            }

            bool DownlinkBuffer::validateMessageReady(DownlinkPacket &downlinkPacket) {
                /*
                TODO
                1. END-flag loydyttava. Miten se tallennetaan bufferiin? CRUCIAL!!!
                2. Kay payloadBuffer lapi ja katso ettei sequencessa ole virheita.
                */
                
                std::string downlinkPayload;
                for (auto const& packet : downlinkPacket.payloadBuffer) {
                    downlinkPayload += packet.second;
                }
                
                return true;
            }
        }
    }
}