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
                u_char flags;
                uint32_t package_id;
                uint16_t sender_id;
                char recipient_id[RECIPIENT_ID_MAX_SIZE];
                uint16_t package_payload_length;
                uint8_t payload_seq;
                long payload_chks;
            };

            void htonSanlaPacketHeader(SanlaPacketHeader header, char buffer[sizeof(SanlaPacketHeader)]) {
                u_char flags;
                flags = htons(header.flags);
                memcpy(buffer+0, &flags, sizeof(u_char));

                uint32_t package_id;
                package_id = htonl(header.package_id);
                memcpy(buffer+1, &package_id, sizeof(uint32_t));

                uint16_t sender_id;
                sender_id = htons(header.sender_id);
                memcpy(buffer+5, &sender_id, sizeof(uint16_t));
                
                char recipient_id[RECIPIENT_ID_MAX_SIZE];
                memcpy(buffer+7, &recipient_id, RECIPIENT_ID_MAX_SIZE);

                uint16_t package_payload_length;
                package_payload_length = htons(header.package_payload_length);
                memcpy(buffer+17, &package_payload_length, sizeof(uint16_t));

                uint8_t payload_seq;
                package_id = htons(header.payload_seq);
                memcpy(buffer+19, &payload_seq, sizeof(uint16_t));
            };

            struct SanlaPacketBody {
                char payload[PACKET_BODY_MAX_SIZE];
                // TODO human readable sender name?
            };

            void htonSanlaPacketBody(SanlaPacketBody body, char buffer[PACKET_BODY_MAX_SIZE]) {
                char payload[PACKET_BODY_MAX_SIZE];
                memcpy(buffer+0, &payload, PACKET_BODY_MAX_SIZE);
            };

            struct SanlaPacket {
                SanlaPacketHeader header;
                SanlaPacketBody body;
            };

            void htonSanlaPacket(SanlaPacketHeader header, SanlaPacketBody body, char buffer[41]) {
                htonSanlaPacketHeader(header, buffer+0);
                htonSanlaPacketBody(body, buffer+21);
            };

        };
};
#endif