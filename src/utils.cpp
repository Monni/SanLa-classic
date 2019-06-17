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
    
    }
}