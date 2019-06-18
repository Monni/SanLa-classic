#include "common/utils.hpp"

namespace sanla {
    namespace messaging {
        
        SanlaMessagePackage downlinkpacketToSanlamessage(DownlinkPacket dl_packet) {

            // TODO what was agreed to do with sender? DownlinkPacket is missing sender information.
            uint16_t dummy_sender;
            dummy_sender = 65535;

            std::string dl_packet_payload;
            for (auto const& str : dl_packet.payloadBuffer) {
                dl_packet_payload += str;
            }
            sanlamessage::Payload_t payload;
            strcpy(payload, dl_packet_payload.c_str());

            return {
                dl_packet.message_id,
                dummy_sender,
                dl_packet.payload_chks,
                dl_packet.recipient_id,
                payload
            };
        };

        DownlinkPacket sanlapacketToDownlinkpacket(SanlaPacket packet) {
            DownlinkPacket dl_packet;
            
            dl_packet.message_id = packet.header.message_id;
            memcpy(dl_packet.recipient_id, packet.header.recipient_id, sanla::messaging::RECIPIENT_ID_MAX_SIZE);
            dl_packet.message_payload_length = packet.header.message_payload_length;
            dl_packet.payload_chks = packet.header.payload_chks;                
            std::string body_string(packet.body);
            dl_packet.payloadBuffer.push_back(body_string);

            return dl_packet;
        };

        SanlaPacket buildBroadcastPacket(SanlaMessagePackage& messagePackage){
            auto msg_header = messagePackage.GetPackageHeader();
            return SanlaPacket {{BRO, msg_header.message_id, msg_header.sender_id,
            *msg_header.recipient_id, 0, 0}, "\0"};
        }

        void htonSanlaPacket(SanlaPacket packet, sanlapacket::SerializedPacket_t buffer) {
            htonSanlaPacketHeader(packet.header, buffer+0);
            memcpy(packet.body, buffer+21, sanlapacket::PACKET_BODY_MAX_SIZE);
        };

        void htonSanlaPacketHeader(SanlaPacketHeader header, sanlapacket::SerializedPacketHeader_t buffer) {
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
    
        SanlaPacketHeader ntohSanlaPacketHeader(sanlapacket::SerializedPacketHeader_t buffer) {
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
    
        
        SanlaPacket ntohSanlaPacket(sanlapacket::SerializedPacket_t buffer) {
            SanlaPacket sanlapacket;

            // Extract header information from incoming serialized data.
            sanlapacket::SerializedPacket_t headerArr;
            for(int i = 0; i < sanlapacket::PACKET_HEADER_SIZE; i++) {
                headerArr[i] = buffer[i];
            };
            sanlapacket.header = sanla::messaging::ntohSanlaPacketHeader(headerArr);

            // Rest of the serialized data belongs to a packet body.
            memcpy(sanlapacket.body, buffer+sanlapacket::PACKET_HEADER_SIZE, sizeof(sanlapacket::Payload_t));

            return sanlapacket;
        }
    
    }
}