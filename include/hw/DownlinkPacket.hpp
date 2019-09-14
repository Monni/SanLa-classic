#ifndef SANLACLASSIC_COMMON_DOWNLINKPACKET_H_
#define SANLACLASSIC_COMMON_DOWNLINKPACKET_H_

#include <map>
#include <vector>
#include "common/typedefs.hpp"
#include "common/SanlaPacket.hpp"
#include "constants.hpp"

namespace sanla {
    namespace hw_interfaces {
        namespace mq {

            struct DownlinkPacket {
                MessageId_t message_id;
                RecipientId_t recipient_id;
                std::map<PayloadSeq_t, std::string> payloadBuffer;
            };

        };
    };
};
#endif