#include "hw/DownlinkBuffer.hpp"
#include "constants.hpp"
#include "common/SanlaProcessor.hpp"
#include "common/utils.hpp"

namespace sanla {
    namespace hw_interfaces {
        namespace mq {

            void DownlinkBuffer::ReceivePacket(SanlaPacket &packet) {

                if (!validatePacket(packet)) {
                   requestMissingPacket(packet.header.message_id, packet.header.payload_seq);
                   return;
                }
                if (messageExistsInStore(packet.header.message_id)) {
                    Serial.println("Message exists in store. Dropping packet.");
                    (void)packet;
                    return;
                }
                StorePacket(packet);
            }

            size_t DownlinkBuffer::GetBufferLength() {
                return downlinkPacketBuffer.size();
            }

            bool DownlinkBuffer::StorePacket(SanlaPacket &packet) {
                std::map<MessageId_t, DownlinkPacket*>::iterator it;
                it = downlinkPacketBuffer.find(packet.header.message_id);
                if (it != downlinkPacketBuffer.end()) {
                    Serial.println("DownlinkBuffer::StorePacket -- Existing DownlinkPacket found.");

                    DownlinkPacket *dl_packet = it->second;

                    if(dl_packet == nullptr){
                        Serial.println("Null packet encountered! FIXME");
                        return false;
                    }

                    // Push payload to Map.
                    std::string body_string(packet.body);
                    dl_packet->payloadBuffer.insert(
                        std::make_pair(std::make_pair(packet.header.payload_seq, packet.header.flags), body_string)
                    );

                    // Validate if DownlinkPacket is ready to be turned into SanlaMessage and send to MessageStore.
                    // TODO this needs to handled on both cases.
                    if (validateMessageReady(*dl_packet)) {
                        Serial.println("DownlinkBuffer::StorePacket -- DownlinkPacket ready to MessageStore.");

                        SanlaMessage message(messaging::downlinkpacketToSanlamessage(*dl_packet));
                        SendMessageToStore(message);
                        downlinkPacketBuffer.erase(packet.header.message_id);
                        delete dl_packet;
                    }   

                } else {
                    Serial.println("DownlinkBuffer::StorePacket -- DownlinkPacket does not exist. Creating.");

                    if (downlinkPacketBuffer.size() >= DOWNLINKBUFFER_MAX_SIZE) {
                        Serial.println("DownlinkBuffer::StorePacket -- DownlinkBuffer is full!");
                        (void)packet;
                        return false;
                    }
                    downlinkPacketBuffer.insert(
                        std::pair<MessageId_t, DownlinkPacket*>(
                            packet.header.message_id,
                            new DownlinkPacket(messaging::sanlapacketToDownlinkpacket(packet)
                            )
                        )
                    );
                }

                // Packet is either stored to downlinkPacketBuffer either way at this point.
                // We can drop the packet here.
                (void)packet;
                return true;
            }

            void DownlinkBuffer::SendMessageToStore(SanlaMessage &message){
                Serial.println("DownlinkBuffer::SendMessageToStore");
                if (m_sanla_processor_ptr != nullptr) {
                    auto processor = static_cast<SanlaProcessor*>(m_sanla_processor_ptr);
                    processor->HandleMessage(message);
                }
                else {
                    // Throw some error here
                }
            }

            bool DownlinkBuffer::messageExistsInStore(MessageId_t messageId) {
                if (m_sanla_processor_ptr != nullptr) {
                    auto processor = static_cast<SanlaProcessor*>(m_sanla_processor_ptr);
                    return processor->messageExistsInStore(messageId);
                } else {
                    // Throw some error here
                }
                return false;
            }

            bool DownlinkBuffer::validatePacket(SanlaPacket &packet) {
                /*
                TODO
                Validate checksum
                */

                if (packet.header.payload_length == strlen(packet.body)) {
                    Serial.println("DownlinkBuffer::validatePacket - true.");
                    return true;
                }

                Serial.println("DownlinkBuffer::validatePacket - false.");
                return false;
            }

            bool DownlinkBuffer::validateMessageReady(DownlinkPacket &dl_packet) {
                bool endFlagFound = false;
                std::vector<PayloadSeq_t> missing_packets;

                PayloadSeq_t expectedPacketStartSeq = 0;
                for (auto const& payloadBuffer: dl_packet.payloadBuffer) {

                    if (payloadBuffer.first.second == messaging::END) {
                        endFlagFound = true;
                    }

                    if (payloadBuffer.first.first > expectedPacketStartSeq) {
                        for (int i = expectedPacketStartSeq; i < payloadBuffer.first.first; i += messaging::sanlapacket::PACKET_BODY_MAX_SIZE-1) {
                            missing_packets.push_back(i);
                        }
                    }
                    expectedPacketStartSeq += (payloadBuffer.first.first + messaging::sanlapacket::PACKET_BODY_MAX_SIZE-1);
                }


                if (!missing_packets.empty()) {
                    if (endFlagFound) {
                        for (auto seq: missing_packets) {
                            requestMissingPacket(dl_packet.message_id, seq);
                        }
                        return false;
                    }
                    return false;
                } else if (!endFlagFound) {
                    return false;
                }

                return true;
                }
        
            void DownlinkBuffer::requestMissingPacket(MessageId_t messageId, PayloadSeq_t payloadSequence) {
                Serial.print("Requesting missing packet ");
                Serial.println(payloadSequence);

                if (m_sanla_processor_ptr != nullptr) {
                    auto processor = static_cast<SanlaProcessor*>(m_sanla_processor_ptr);
                    SanlaPacket packet = messaging::buildRequestPacket(messageId, payloadSequence);
                    processor->HandlePacket(packet);
                }
                else {
                    // Throw some error here
                }
            }

        }
    }
}