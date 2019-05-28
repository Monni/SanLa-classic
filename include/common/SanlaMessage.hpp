#ifndef SANLACLASSIC_COMMON_SANLAMESSAGE_H_
#define SANLACLASSIC_COMMON_SANLAMESSAGE_H_

#include <stdint.h>
#include <string>
#include <cstddef>
#include <sstream>
#include "constants.hpp"
#include "common/typedefs.hpp"
#include "hw/DownlinkPacket.hpp"

namespace sanla {
    struct foo {
        MessageId_t asd;
    };

    namespace asd {struct MessageHeader {
            MessageId_t message_id;
            SenderId_t sender_id;
            PayloadChecksum_t payload_chks;
            RecipientId_t recipient_id;
        };};
    namespace messaging {

        struct MessageHeader {
            MessageId_t message_id;
            SenderId_t sender_id;
            PayloadChecksum_t payload_chks;
            RecipientId_t recipient_id;
        };

        using Payload_t = messaging::sanlamessage::Payload_t;
        using DownlinkPacket = hw_interfaces::mq::DownlinkPacket;

        class SanlaMessagePackage {
            MessageHeader header;
            Payload_t body;

            public:
            SanlaMessagePackage(MessageId_t, SenderId_t, PayloadChecksum_t, RecipientId_t, Payload_t);
            SanlaMessagePackage(MessageHeader, Payload_t);
            SanlaMessagePackage(DownlinkPacket);
            // This class is not moveable
            SanlaMessagePackage(SanlaMessagePackage&&) = delete;
            SanlaMessagePackage& operator=(SanlaMessagePackage&&) = delete;
            // This class is not copyable
            SanlaMessagePackage(const SanlaMessagePackage&) = delete;
            SanlaMessagePackage& operator=(const SanlaMessagePackage&) = delete;

            ~SanlaMessagePackage(){};

            uint16_t GetPackageLength();
            MessageHeader& GetPackageHeader();
            Payload_t& GetPackageBody();
        };

    }; // messaging
}; // sanla
#endif