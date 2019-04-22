#include <netinet/in.h>
#include <stdint.h>
#include <string>
#include "constants.hpp"
#include "common/typedefs.hpp"
#include <iostream>
#include <cstdint>
#include <cstring>
#include "common/SanlaMessage.hpp"

#ifndef SANLACLASSIC_COMMON_SANLAPACKET_H_
#define SANLACLASSIC_COMMON_SANLAPACKET_H_

namespace sanla {
    namespace sanlamessage {

        const Flag_t BRO {0x1}, REQ {0x2}, PRO {0x3}, PAC {0x4}, ACK {0x8},
        SEN {0xC}, RES {0xA}, PACREQ {0x6}, PACPRO {0x7}, PROACK {0xB},
        PACSEN {0xF}, PACRES {0xD};

            struct SanlaPacketHeader {
                Flag_t flags;
                PackageId_t package_id;
                SenderId_t sender_id;
                RecipientId_t recipient_id;
                PayloadLength_t package_payload_length;
                PayloadSeq_t payload_seq;
                PayloadChecksum_t payload_chks;
            };

            struct SanlaPacket {
                SanlaPacketHeader header;
                Payload_t body;
                
                void copy_headers_from_message(MessageHeader, MessageBody);
            };

            inline void htonSanlaPacketHeader(SanlaPacketHeader, char[23]);
            inline SanlaPacketHeader ntohSanlaPacketHeader(char[23]);
            inline void htonSanlaPacket(SanlaPacket, char[41]);
            inline SanlaPacket ntohSanlaPacket(char[41]);

        };
};
#endif