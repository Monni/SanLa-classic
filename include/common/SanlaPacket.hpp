#include <arpa/inet.h>
#include <stdint.h>
#include <string>
#include "constants.hpp"
#include <iostream>
#include <cstdint>
#include <cstring>

#ifndef SANLACLASSIC_COMMON_SANLAPACKET_H_
#define SANLACLASSIC_COMMON_SANLAPACKET_H_

namespace sanla {
    namespace sanlamessage {
            const u_char BRO {0x1}, REQ {0x2}, PRO {0x3}, PAC {0x4}, ACK {0x8},
            SEN {0xC}, RES {0xA}, PACREQ {0x6}, PACPRO {0x7}, PROACK {0xB},
            PACSEN {0xF}, PACRES {0xD};

            struct SanlaPacketHeader {
                uint8_t flags;
                uint32_t package_id;
                uint16_t sender_id;
                char recipient_id[RECIPIENT_ID_MAX_SIZE];
                uint16_t package_payload_length;
                uint16_t payload_seq;
                uint32_t payload_chks;
            };

            inline void htonSanlaPacketHeader(SanlaPacketHeader header, char buffer[23]) {
                memcpy(buffer+0, &header.flags, 1);
                
                uint32_t package_id = htonl(header.package_id);
                memcpy(buffer+1, &package_id, 4);

                uint16_t sender_id;
                sender_id = htons(header.sender_id);
                memcpy(buffer+5, &sender_id, 2);
                
                char recipient_id[RECIPIENT_ID_MAX_SIZE]{};
                memcpy(buffer+7, &recipient_id, 8); // TODO this may or may not work

                uint16_t package_payload_length = htons(header.package_payload_length);
                memcpy(buffer+15, &package_payload_length, 2);

                uint16_t payload_seq = htons(header.payload_seq);
                memcpy(buffer+17, &payload_seq, 2);

                uint32_t payload_chks = htonl(header.payload_chks);
                memcpy(buffer+19, &payload_chks, 4);
            };

            struct SanlaPacketBody {
                char payload[PACKET_BODY_MAX_SIZE];
                // TODO human readable sender name?
            };

            inline void htonSanlaPacketBody(SanlaPacketBody body, char buffer[PACKET_BODY_MAX_SIZE]) {
                char payload[PACKET_BODY_MAX_SIZE];
                memcpy(buffer+0, &payload, PACKET_BODY_MAX_SIZE);
            };

            struct SanlaPacket {
                SanlaPacketHeader header;
                SanlaPacketBody body;
            };

            inline void htonSanlaPacket(SanlaPacketHeader header, SanlaPacketBody body, char buffer[41]) {
                htonSanlaPacketHeader(header, buffer+0);
                htonSanlaPacketBody(body, buffer+21);
            };

        };
};
#endif