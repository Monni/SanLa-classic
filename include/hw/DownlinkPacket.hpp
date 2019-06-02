#ifndef SANLACLASSIC_COMMON_DOWNLINKPACKET_H_
#define SANLACLASSIC_COMMON_DOWNLINKPACKET_H_

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
                PayloadLength_t message_payload_length;
                PayloadChecksum_t payload_chks;
                std::vector<std::string> payloadBuffer;
            };

        };
    };
};
#endif