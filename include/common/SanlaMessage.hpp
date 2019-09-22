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
            RecipientId_t recipient_id;
        };

        using Payload_t = messaging::sanlamessage::Payload_t;
        using PacketPayload_t = messaging::sanlapacket::Payload_t;

        class SanlaMessage {
            MessageHeader header;
            Payload_t body;

            public:
            SanlaMessage(MessageId_t, SenderId_t, RecipientId_t, Payload_t);
            SanlaMessage(MessageHeader, Payload_t);

            ~SanlaMessage(){};

            uint16_t GetMessageLength();
            MessageHeader& GetMessageHeader();
            Payload_t& GetMessageBody();
        };

    }; // messaging
}; // sanla
#endif