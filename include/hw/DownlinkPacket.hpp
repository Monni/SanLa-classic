#ifndef SANLACLASSIC_COMMON_DOWNLINKPACKET_H_
#define SANLACLASSIC_COMMON_DOWNLINKPACKET_H_

#include <map>
#include <vector>
#include "common/typedefs.hpp"
#include "common/SanlaPacket.hpp"
#include "constants.hpp"
#include "HardwareSerial.h"

namespace sanla {
    namespace hw_interfaces {
        namespace mq {
            using PayloadBuffer_t = messaging::dl_sanlapacket::PayloadBuffer_t;

            struct DownlinkPacket {
                MessageId_t message_id;
                SenderId_t sender_id;
                RecipientId_t recipient_id;
                PayloadBuffer_t payloadBuffer;
            };
        };
    };
};
#endif