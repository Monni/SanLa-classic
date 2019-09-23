#include "common/utils.hpp"

namespace sanla {
    namespace messaging {
        
        SanlaMessage downlinkpacketToSanlamessage(DownlinkPacket &dl_packet) {
            std::string dl_packet_payload;
            for (auto const& payload_buffer : dl_packet.payloadBuffer) {
                dl_packet_payload += payload_buffer.second.c_str();
            }
            sanlamessage::Payload_t payload;
            strcpy(payload, dl_packet_payload.c_str());

            return SanlaMessage(
                dl_packet.message_id,
                dl_packet.sender_id,
                dl_packet.recipient_id,
                payload
            );
        };

        DownlinkPacket sanlapacketToDownlinkpacket(SanlaPacket &packet) {
            DownlinkPacket dl_packet;

            dl_packet.message_id = packet.header.message_id;
            dl_packet.recipient_id = packet.header.recipient_id;                
            std::string body_string(packet.body);

            dl_packet.payloadBuffer.insert(
                std::make_pair(std::make_pair(packet.header.payload_seq, packet.header.flags), body_string)
            );

            return dl_packet;
        };

        std::vector<SanlaPacket> buildPacketsFromMessage(SanlaMessage& message){
            std::vector<SanlaPacket> packet_vector;
            std::string message_body(message.GetMessageBody());

            for (unsigned i = 0; i < message_body.length(); i += sanla::messaging::sanlapacket::PACKET_BODY_MAX_SIZE-1) {
                SanlaPacket packet{};

                char payloadArr[sanlapacket::PACKET_BODY_MAX_SIZE];
                strncpy(payloadArr, message_body.substr(i, sanla::messaging::sanlapacket::PACKET_BODY_MAX_SIZE-1).c_str(), sanla::messaging::sanlapacket::PACKET_BODY_MAX_SIZE);

                packet.copy_headers_from_message(message.GetMessageHeader(), payloadArr); // TODO total header + payload, nyt menee pelkka payload.
                packet.header.payload_seq = i;
                memcpy(&packet.body, payloadArr, sizeof(payloadArr));

                // Mark last packet with END flag.
                if (i + sanla::messaging::sanlapacket::PACKET_BODY_MAX_SIZE-1 >= message_body.length()) {
                    packet.header.flags = END;
                }

                packet_vector.push_back(packet);
            }

            return packet_vector;
        }

        SanlaPacket buildRequestPacket(MessageId_t messageId, PayloadSeq_t payloadSequence) {
            SanlaPacket packet{};

            packet.header.message_id = messageId;
            packet.header.payload_seq = payloadSequence;
            packet.header.flags = messaging::REQ;

            return packet;
        }

        void htonSanlaPacket(SanlaPacket &packet, sanlapacket::SerializedPacket_t buffer) {
            htonSanlaPacketHeader(packet.header, buffer+0);
            strcpy(buffer+sanlapacket::PACKET_HEADER_SIZE, packet.body);
        };

        void htonSanlaPacketHeader(SanlaPacketHeader header, sanlapacket::SerializedPacketHeader_t buffer) {
            SenderId_t sender_id; // 2
            sender_id = htons(header.sender_id);
            memcpy(buffer+0, &sender_id, sizeof(header.sender_id));

            RecipientId_t recipient_id; // 2
            recipient_id = htons(header.recipient_id);
            memcpy(buffer+2, &recipient_id, sizeof(header.recipient_id));

            memcpy(buffer+4, &header.flags, sizeof(header.flags)); // 1

            memcpy(buffer+5, &header.payload_length, sizeof(header.payload_length)); // 1

            PayloadChecksum_t payload_chks = htons(header.payload_chks); // 2
            memcpy(buffer+6, &payload_chks, sizeof(header.payload_seq));

            MessageId_t message_id = htons(header.message_id); // 2
            memcpy(buffer+8, &message_id, sizeof(message_id));

            PayloadSeq_t payload_seq = htons(header.payload_seq); // 2
            memcpy(buffer+10, &payload_seq, sizeof(header.payload_seq));            
        };
    
        SanlaPacketHeader ntohSanlaPacketHeader(sanlapacket::SerializedPacketHeader_t buffer) {
            SanlaPacketHeader tmp;

            memcpy(&tmp.sender_id, buffer+0, sizeof(tmp.sender_id)); // 2
            tmp.sender_id = ntohs(tmp.sender_id);

            memcpy(&tmp.recipient_id, buffer+2, sizeof(tmp.recipient_id)); // 2
            tmp.recipient_id = ntohs(tmp.recipient_id);
            
            memcpy(&tmp.flags, buffer+4, sizeof(tmp.flags)); // 1
            
            memcpy(&tmp.payload_length, buffer+5, sizeof(tmp.payload_length)); // 1

            memcpy(&tmp.payload_chks, buffer+6, sizeof(tmp.payload_chks)); // 2
            tmp.payload_chks = ntohs(tmp.payload_chks);

            memcpy(&tmp.message_id, buffer+8, sizeof(tmp.message_id)); // 2
            tmp.message_id = ntohs(tmp.message_id);

            memcpy(&tmp.payload_seq, buffer+10, sizeof(tmp.payload_seq)); // 2
            tmp.payload_seq = ntohs(tmp.payload_seq);

            return tmp;
        };
    
        
        SanlaPacket ntohSanlaPacket(sanlapacket::SerializedPacket_t buffer) {
            SanlaPacket sanlapacket;

            // Extract header information from incoming serialized data.
            sanlapacket::SerializedPacket_t headerArr;
            for(int i = 0; i < sanlapacket::PACKET_HEADER_SIZE; i++) {
                headerArr[i] = buffer[i];
            };
            sanlapacket.header = sanla::messaging::ntohSanlaPacketHeader(headerArr);

            // Rest of the serialized data belongs to a packet body.
            strcpy(sanlapacket.body, buffer+sanlapacket::PACKET_HEADER_SIZE);

            return sanlapacket;
        }
    
    }
}