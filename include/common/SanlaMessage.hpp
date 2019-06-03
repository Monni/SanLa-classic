#ifndef SANLACLASSIC_COMMON_SANLAMESSAGE_H_
#define SANLACLASSIC_COMMON_SANLAMESSAGE_H_

#include <stdint.h>
#include <string>
#include <cstddef>
#include <sstream>
#include "constants.hpp"
#include "common/typedefs.hpp"

namespace sanla {
    namespace messaging {

        struct MessageHeader {
            MessageId_t message_id;
            SenderId_t sender_id;
            PayloadChecksum_t payload_chks;
            RecipientId_t recipient_id;
        };

        using Payload_t = messaging::sanlamessage::Payload_t;

        class SanlaMessagePackage {
            MessageHeader header;
            Payload_t body;

            public:
            SanlaMessagePackage(MessageId_t, SenderId_t, PayloadChecksum_t, RecipientId_t, Payload_t);
            SanlaMessagePackage(MessageHeader, Payload_t);

            ~SanlaMessagePackage(){};

            uint16_t GetPackageLength();
            MessageHeader& GetPackageHeader();
            Payload_t& GetPackageBody();
        };

    }; // messaging
}; // sanla
#endif