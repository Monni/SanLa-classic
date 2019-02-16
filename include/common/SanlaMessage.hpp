#include <stdint.h>
#include <string>
#include <cstddef>
#include <sstream>
#include "constants.hpp"


#ifndef SANLACLASSIC_COMMON_SANLAMESSAGE_H_
#define SANLACLASSIC_COMMON_SANLAMESSAGE_H_

namespace sanla {

    struct MessageBody {
        char sender[sanla::sanlamessage::MESSAGE_BODY_SENDER_MAX_SIZE];
        char payload[sanla::sanlamessage::MESSAGE_BODY_PAYLOAD_MAX_SIZE];
    };

    struct MessageHeader {
        uint32_t package_id;
        uint16_t sender_id;
        uint32_t payload_chks;
        //std::string recipient_id;
    };

    class SanlaMessagePackage {
        MessageHeader header;
        MessageBody body;

        public:
        SanlaMessagePackage(uint32_t, uint16_t, uint32_t, MessageBody);
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