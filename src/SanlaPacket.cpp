#include "common/SanlaPacket.hpp"

namespace sanla {
    namespace sanlamessage {

        void htonSanlaPacketHeader(SanlaPacketHeader header, char buffer[23]) {
            memcpy(buffer+0, &header.flags, sizeof(header.flags));
            
            PackageId_t package_id = htonl(header.package_id);
            memcpy(buffer+1, &package_id, sizeof(package_id));

            SenderId_t sender_id;
            sender_id = htons(header.sender_id);
            memcpy(buffer+5, &sender_id, sizeof(header.sender_id));
            
            RecipientId_t recipient_id{};
            memcpy(buffer+7, &recipient_id, RECIPIENT_ID_MAX_SIZE); // TODO this may or may not work

            PayloadLength_t package_payload_length = htons(header.package_payload_length);
            memcpy(buffer+15, &package_payload_length, sizeof(header.package_payload_length));

            PayloadSeq_t payload_seq = htons(header.payload_seq);
            memcpy(buffer+17, &payload_seq, sizeof(header.payload_seq));

            PayloadChecksum_t payload_chks = htonl(header.payload_chks);
            memcpy(buffer+19, &payload_chks, sizeof(header.payload_seq));
        };

        // Network to host
        inline SanlaPacketHeader ntohSanlaPacketHeader(char buffer[23]) {
            SanlaPacketHeader tmp;
            
            memcpy(&tmp.flags, buffer+0, sizeof(tmp.flags));
            
            memcpy(&tmp.package_id, buffer+1, sizeof(tmp.package_id));
            tmp.package_id = ntohl(tmp.package_id);

            memcpy(&tmp.sender_id, buffer+5, sizeof(tmp.sender_id));
            tmp.sender_id = ntohs(tmp.sender_id);
            
            memcpy(&tmp.recipient_id, buffer+7, RECIPIENT_ID_MAX_SIZE); // TODO this may or may not work

            memcpy(&tmp.package_payload_length, buffer+15, sizeof(tmp.package_payload_length));
            tmp.package_payload_length = ntohs(tmp.package_payload_length);

            memcpy(&tmp.payload_seq, buffer+17, sizeof(tmp.payload_seq));
            tmp.payload_seq = ntohs(tmp.payload_seq);

            memcpy(&tmp.payload_chks, buffer+19, sizeof(tmp.payload_chks));
            tmp.payload_chks = ntohl(tmp.payload_chks);

            return tmp;
        };

        void htonSanlaPacketBody(SanlaPacketBody body, char buffer[PACKET_BODY_MAX_SIZE]) {
            char payload[PACKET_BODY_MAX_SIZE];
            memcpy(buffer+0, &payload, PACKET_BODY_MAX_SIZE);
        };
    
        void htonSanlaPacket(SanlaPacketHeader header, SanlaPacketBody body, char buffer[41]) {
            htonSanlaPacketHeader(header, buffer+0);
            htonSanlaPacketBody(body, buffer+21);
        };

    }
}