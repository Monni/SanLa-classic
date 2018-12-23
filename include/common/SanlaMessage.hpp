#include <stdint.h>
#include <string>
#include <cstddef>


#ifndef SANLACLASSIC_COMMON_SANLAMESSAGE_H_
#define SANLACLASSIC_COMMON_SANLAMESSAGE_H_

namespace sanla {
    namespace sanlamessage {
        const uint8_t RECIPIENT_ID_MAX_SIZE {16};
        const u_char BRO {0x1}, REQ {0x2}, PRO {0x3}, PAC {0x4}, ACK {0x8},
        SEN {0xC}, RES {0xA}, PACREQ {0x6}, PACPRO {0x7}, PROACK {0xB},
        PACSEN {0xF}, PACRES {0xD};

        struct SanlaPacket {
            u_char flags;
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
        const char *payload;
    };

    struct MessageHeader {
        u_char flags;
        uint8_t payload_seq;
        uint16_t length;
        uint64_t sender_id, payload_chks;
        uint32_t package_id;
        std::string recipient_id;
    };

    class SanlaMessagePackage {
        MessageHeader header;
        MessageBody body;

        public:
        SanlaMessagePackage(u_char, uint8_t, uint16_t, 
        uint64_t, uint64_t, uint32_t, std::string, MessageBody);
        SanlaMessagePackage(MessageHeader, MessageBody);
        // This class is not moveable
        SanlaMessagePackage(SanlaMessagePackage&&) = delete;
        SanlaMessagePackage& operator=(SanlaMessagePackage&&) = delete;
        // This class is not copyable
        SanlaMessagePackage(const SanlaMessagePackage&) = delete;
        SanlaMessagePackage& operator=(const SanlaMessagePackage&) = delete;

        ~SanlaMessagePackage(){};

        uint8_t GetPackageLength();
        uint16_t GetTotalPackageLength();
        MessageHeader& GetPackageHeader();
        MessageBody& GetPackageBody();
    };
};
#endif