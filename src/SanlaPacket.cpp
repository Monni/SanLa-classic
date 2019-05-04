#include "common/SanlaPacket.hpp"

namespace sanla {
    namespace messaging {

        void htonSanlaPacketHeader(SanlaPacketHeader header, char buffer[23]) {
            memcpy(buffer+0, &header.flags, sizeof(header.flags));
            
            MessageId_t message_id = htonl(header.message_id);
            memcpy(buffer+1, &message_id, sizeof(message_id));

            SenderId_t sender_id;
            sender_id = htons(header.sender_id);
            memcpy(buffer+5, &sender_id, sizeof(header.sender_id));
            
            RecipientId_t recipient_id{};
            memcpy(buffer+7, &recipient_id, RECIPIENT_ID_MAX_SIZE); // TODO this may or may not work

            PayloadLength_t message_payload_length = htons(header.message_payload_length);
            memcpy(buffer+15, &message_payload_length, sizeof(header.message_payload_length));

            PayloadSeq_t payload_seq = htons(header.payload_seq);
            memcpy(buffer+17, &payload_seq, sizeof(header.payload_seq));

            PayloadChecksum_t payload_chks = htonl(header.payload_chks);
            memcpy(buffer+19, &payload_chks, sizeof(header.payload_seq));
        };

        // Network to host
        inline SanlaPacketHeader ntohSanlaPacketHeader(char buffer[23]) {
            SanlaPacketHeader tmp;
            
            memcpy(&tmp.flags, buffer+0, sizeof(tmp.flags));
            
            memcpy(&tmp.message_id, buffer+1, sizeof(tmp.message_id));
            tmp.message_id = ntohl(tmp.message_id);

            memcpy(&tmp.sender_id, buffer+5, sizeof(tmp.sender_id));
            tmp.sender_id = ntohs(tmp.sender_id);
            
            memcpy(&tmp.recipient_id, buffer+7, RECIPIENT_ID_MAX_SIZE); // TODO this may or may not work

            memcpy(&tmp.message_payload_length, buffer+15, sizeof(tmp.message_payload_length));
            tmp.message_payload_length = ntohs(tmp.message_payload_length);

            memcpy(&tmp.payload_seq, buffer+17, sizeof(tmp.payload_seq));
            tmp.payload_seq = ntohs(tmp.payload_seq);

            memcpy(&tmp.payload_chks, buffer+19, sizeof(tmp.payload_chks));
            tmp.payload_chks = ntohl(tmp.payload_chks);

            return tmp;
        };
    
        void htonSanlaPacket(SanlaPacket packet, sanlapacket::SerializedPacket_t buffer) {
            htonSanlaPacketHeader(packet.header, buffer+0);
            memcpy(packet.body, buffer+21, sanlapacket::PACKET_BODY_MAX_SIZE);
        };

        inline SanlaPacket ntohSanlaPacket(sanlapacket::SerializedPacket_t buffer) {
            SanlaPacket sanlapacket;

            char headerArr[23];
            for(int i = 0; i < 23; i++) {
                headerArr[i] = buffer[i];
            };
            sanlapacket.header = sanla::messaging::ntohSanlaPacketHeader(headerArr);

            memcpy(sanlapacket.body, buffer+23, sizeof(sanlapacket::Payload_t));

            return sanlapacket;
        }

    }
}