#include <stdint.h>
#include <string>
#include <cstddef>


#ifndef SANLACLASSIC_COMMON_SANLAMESSAGE_H_
#define SANLACLASSIC_COMMON_SANLAMESSAGE_H_

namespace sanla {
    namespace sanlamessage {
        const uint8_t RECIPIENT_ID_MAX_SIZE {16};
        const std::byte BRO {0x1}, REQ {0x2}, PRO {0x3}, PAC {0x4}, ACK {0x8},
        SEN {0xC}, RES {0xA}, PACREQ {0x6}, PACPRO {0x7}, PROACK {0xB},
        PACSEN {0xF}, PACRES {0xD};

        struct SanlaPacket {
            std::byte flags;
            long package_id;
            uint16_t sender_id;
            char recipient_id[RECIPIENT_ID_MAX_SIZE];
            uint16_t package_payload_length;
            uint8_t payload_seq;
            long payload_chks;
            char* payload;
        };
    };

    struct MessageBody {
        char* sender; // TODO: use [max_size] instead of pointer, also define max_size
        char* payload;
    };

    // TODO decide if this is copyable and/or moveable or neither
    // see: https://google.github.io/styleguide/cppguide.html#Copyable_Movable_Types
    class SanlaMessagePackage {
        uint8_t flags, payload_seq;
        uint16_t sender_id, length;
        long package_id, payload_chks;
        std::string recipient_id;
        MessageBody body;

        public:
        SanlaMessagePackage (uint8_t, uint8_t, uint16_t, 
        uint16_t, long, long, std::string, MessageBody);
        ~SanlaMessagePackage ();
    };
};
#endif