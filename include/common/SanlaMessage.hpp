#include <stdint.h>
#include <string>
#include <cstddef>
#include <sstream>


#ifndef SANLACLASSIC_COMMON_SANLAMESSAGE_H_
#define SANLACLASSIC_COMMON_SANLAMESSAGE_H_

namespace sanla {

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
        uint16_t, uint32_t, uint32_t, std::string, MessageBody);
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