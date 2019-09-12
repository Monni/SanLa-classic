#ifndef SANLACLASSIC_COMMON_SANLAPACKET_H_
#define SANLACLASSIC_COMMON_SANLAPACKET_H_

#include "lwip/ip4_addr.h"
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
        
        const Flags_t BRO {0x1}, REQ {0x2}, PRO {0x3}, PAC {0x4}, ACK {0x8},
        SEN {0xC}, RES {0xA}, PACREQ {0x6}, PACPRO {0x7}, PROACK {0xB},
        PACSEN {0xF}, PACRES {0xD};

        struct SanlaPacketHeader {
            Flags_t flags;
            MessageId_t message_id;
            SenderId_t sender_id;
            RecipientId_t recipient_id;
            PayloadLength_t payload_length;
            PayloadSeq_t payload_seq;
            PayloadChecksum_t payload_chks;
        };

        struct SanlaPacket {
            SanlaPacketHeader header;
            sanlapacket::Payload_t body;
            
            void copy_headers_from_message(MessageHeader, sanlamessage::Payload_t);
        };

    };
};
#endif