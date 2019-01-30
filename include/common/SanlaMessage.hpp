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
        uint32_t package_id;
        uint16_t sender_id;
        uint32_t payload_chks;
        std::string recipient_id;
    };

    class SanlaMessagePackage {
        MessageHeader header;
        MessageBody body;

        public:
        SanlaMessagePackage(uint32_t, uint16_t, uint32_t, std::string, MessageBody);
        SanlaMessagePackage(MessageHeader, MessageBody);
        // This class is not moveable
        SanlaMessagePackage(SanlaMessagePackage&&) = delete;
        SanlaMessagePackage& operator=(SanlaMessagePackage&&) = delete;
        // This class is not copyable
        SanlaMessagePackage(const SanlaMessagePackage&) = delete;
        SanlaMessagePackage& operator=(const SanlaMessagePackage&) = delete;

        ~SanlaMessagePackage(){};

        uint8_t GetPackageLength();
        MessageHeader& GetPackageHeader();
        MessageBody& GetPackageBody();
    };
};
#endif