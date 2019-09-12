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
                dl_packet.recipient_id,
                payload
            };
        };

        DownlinkPacket sanlapacketToDownlinkpacket(SanlaPacket packet) {
            DownlinkPacket dl_packet;
            
            dl_packet.message_id = packet.header.message_id;
            dl_packet.recipient_id = packet.header.recipient_id;
            dl_packet.payload_length = packet.header.payload_length;
            dl_packet.payload_chks = packet.header.payload_chks;                
            std::string body_string(packet.body);
            dl_packet.payloadBuffer.push_back(body_string);

            return dl_packet;
        };

        SanlaPacket buildBroadcastPacket(SanlaMessagePackage& messagePackage){
            auto msg_header = messagePackage.GetPackageHeader();
            return SanlaPacket {{BRO, msg_header.message_id, msg_header.sender_id,
            msg_header.recipient_id, 0, 0}, "\0"};
        }

        std::vector<SanlaPacket> buildBroadcastPackets(SanlaMessagePackage& messagePackage){
            auto msg_header = messagePackage.GetPackageHeader();
            // TODO
        }

        void htonSanlaPacket(SanlaPacket packet, sanlapacket::SerializedPacket_t buffer) {
            htonSanlaPacketHeader(packet.header, buffer+0);
            memcpy(packet.body, buffer+21, sanlapacket::PACKET_BODY_MAX_SIZE);
        };

        void htonSanlaPacketHeader(SanlaPacketHeader header, sanlapacket::SerializedPacketHeader_t buffer) {
            SenderId_t sender_id; // 2
            sender_id = htons(header.sender_id);
            memcpy(buffer+0, &sender_id, sizeof(header.sender_id));

            RecipientId_t recipient_id; // 2
            recipient_id = htons(header.recipient_id);
            memcpy(buffer+2, &recipient_id, sizeof(header.recipient_id));

            memcpy(buffer+4, &header.flags, sizeof(header.flags)); // 1
            
            PayloadLength_t payload_length = htons(header.payload_length); // 1
            memcpy(buffer+5, &payload_length, sizeof(header.payload_length));

            PayloadChecksum_t payload_chks = htonl(header.payload_chks); // 2
            memcpy(buffer+6, &payload_chks, sizeof(header.payload_seq));

            MessageId_t message_id = htonl(header.message_id); // 2
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
            tmp.payload_length = ntohs(tmp.payload_length);

            memcpy(&tmp.payload_chks, buffer+6, sizeof(tmp.payload_chks)); // 2
            tmp.payload_chks = ntohl(tmp.payload_chks);

            memcpy(&tmp.message_id, buffer+8, sizeof(tmp.message_id)); // 2
            tmp.message_id = ntohl(tmp.message_id);

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
            memcpy(sanlapacket.body, buffer+sanlapacket::PACKET_HEADER_SIZE, sizeof(sanlapacket::Payload_t));

            return sanlapacket;
        }
    
    }
}