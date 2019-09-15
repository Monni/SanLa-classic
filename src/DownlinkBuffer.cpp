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

            size_t DownlinkBuffer::GetBufferLength() {
                return downlinkPacketBuffer.size();
            }

            bool DownlinkBuffer::StorePacket(SanlaPacket &packet) {
                Serial.println("DownlinkBuffer::StorePacket");

                // Validate incoming packet.
                // TODO if packet is not valid, REQ a new one!
                validatePacket(packet);
                
                std::map<MessageId_t, DownlinkPacket*>::iterator it;
                it = downlinkPacketBuffer.find(packet.header.message_id);
                
                Serial.println("DownlinkBuffer::StorePacket -- trying to find existing dl_packet");
                if (it != downlinkPacketBuffer.end()) {
                    Serial.println("DownlinkBuffer::StorePacket -- dl_packet found.");

                    DownlinkPacket *dl_packet = it->second;

                    // Push payload to Map.
                    std::string body_string(packet.body);

                    Serial.print("payloadBuffer size pre: ");
                    Serial.println(dl_packet->payloadBuffer.size());

                    /* TODO
                    Jos message on valmistunut ja poistettu bufferista,
                    sen jalkeen uusi dl_packet tulee tanne natisti, mutta siihen lisatessa payloadia crash.
                    */
                    dl_packet->payloadBuffer.insert(std::pair<PayloadSeq_t, std::string>(packet.header.payload_seq, body_string));

                    // TODO why this crashes?
                    Serial.print("payloadBuffer size post: ");
                    Serial.println(dl_packet->payloadBuffer.size());
                    Serial.println("Dong finished");
                    
                    // Validate if DownlinkPacket is ready to be turned into SanlaMessage.
                    // TODO this needs to handled on both cases.
                    if (validateMessageReady(dl_packet->payloadBuffer)) {
                        Serial.println("DownlinkBuffer::StorePacket -- dl_packet ready to MessageStore");
                        
                        //Send to store and remove element from Map.
                        SanlaPackage message = messaging::downlinkpacketToSanlamessage(*dl_packet);
                        SendMessageToStore(message);
                        
                        downlinkPacketBuffer.erase(packet.header.message_id);
                        dl_packet = nullptr;
                    }
                    
                
                } else {
                    Serial.println("DownlinkBuffer::StorePacket -- dl_packet not found. Creating.");

                    if (downlinkPacketBuffer.size() >= DOWNLINKBUFFER_MAX_SIZE) {
                        Serial.println("DownlinkBuffer::StorePacket -- DownlinkBuffer is full!");
                        (void)packet;
                        return false;
                    }

                    DownlinkPacket dl_packet = messaging::sanlapacketToDownlinkpacket(packet);
                    downlinkPacketBuffer.insert(std::pair<MessageId_t, DownlinkPacket*>(packet.header.message_id, &dl_packet));
                }

                // Packet is either stored to downlinkPacketBuffer either way at this point.
                // We can drop the packet here.
                (void)packet;
                return true;
            }

            void DownlinkBuffer::SendMessageToStore(SanlaPackage &message){
                Serial.println("DownlinkBuffer::SendMessageToStore");
                if (m_sanla_processor_ptr != nullptr) {
                    auto processor = static_cast<SanlaProcessor*>(m_sanla_processor_ptr);
                    Serial.println("Processor casted");
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
                    Serial.println("DownlinkBuffer::validatePacket - true");
                    return true;
                }

                Serial.println("DownlinkBuffer::validatePacket - false");
                return false;
            }

            bool DownlinkBuffer::validateMessageReady(PayloadBuffer_t &payload_buffer) {
                /*
                TODO
                1. END-flag loydyttava. Miten se tallennetaan bufferiin? CRUCIAL!!!
                2. Kay payloadBuffer lapi ja katso ettei sequencessa ole virheita.
                */
                std::string downlinkPayload;
                //for (auto const& payload : payload_buffer) {
                //}
                
                return true;
            }
        }
    }
}