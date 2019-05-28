#ifndef SANLACLASSIC_COMMON_SANLAPACKET_H_
#define SANLACLASSIC_COMMON_SANLAPACKET_H_

#include <netinet/in.h>
#include <stdint.h>
#include <string>
#include "constants.hpp"
#include "common/typedefs.hpp"
#include <iostream>
#include <cstdint>
#include <cstring>
#include "common/SanlaMessage.hpp"

namespace sanla {
    namespace messaging {
        
        const Flag_t BRO {0x1}, REQ {0x2}, PRO {0x3}, PAC {0x4}, ACK {0x8},
        SEN {0xC}, RES {0xA}, PACREQ {0x6}, PACPRO {0x7}, PROACK {0xB},
        PACSEN {0xF}, PACRES {0xD};

            struct SanlaPacketHeader {
                Flag_t flags;
                MessageId_t message_id;
                SenderId_t sender_id;
                RecipientId_t recipient_id;
                PayloadLength_t message_payload_length;
                PayloadSeq_t payload_seq;
                PayloadChecksum_t payload_chks;
            };

            struct SanlaPacket {
                SanlaPacketHeader header;
                sanlapacket::Payload_t body;
                
                // Todo fix this fucker
                void copy_headers_from_message(sanla::foo, sanlamessage::Payload_t);
            };

            inline void htonSanlaPacketHeader(SanlaPacketHeader, sanlapacket::SerializedPacketHeader_t);
            inline SanlaPacketHeader ntohSanlaPacketHeader(sanlapacket::SerializedPacketHeader_t);
            inline void htonSanlaPacket(SanlaPacket, sanlapacket::SerializedPacket_t);
            inline SanlaPacket ntohSanlaPacket(sanlapacket::SerializedPacket_t);

        };
};
#endif