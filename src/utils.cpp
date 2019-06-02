#include "common/utils.hpp"

namespace sanla {
    namespace messaging {
        
        SanlaMessagePackage downlinkpacket_to_sanlamessage(DownlinkPacket downlink_packet) {

            char foo[2000];
            uint16_t bar;
            bar = 12345;
            
            return SanlaMessagePackage(
                downlink_packet.message_id,
                bar, // TODO what to do with sender id?
                downlink_packet.payload_chks,
                downlink_packet.recipient_id,
                foo, // TODO string vector to char array
            );

        };

        DownlinkPacket sanlapacket_to_downlinkpacket(SanlaPacket packet) {
            DownlinkPacket dl_packet;
            
            dl_packet.message_id = packet.header.message_id;
            memcpy(dl_packet.recipient_id, packet.header.recipient_id, sanla::messaging::RECIPIENT_ID_MAX_SIZE);
            dl_packet.message_payload_length = packet.header.message_payload_length;
            dl_packet.payload_chks = packet.header.payload_chks;                
            std::string body_string(packet.body);
            dl_packet.payloadBuffer.push_back(body_string);

            return dl_packet;
        };

    
    }
}