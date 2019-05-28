#ifndef SANLACLASSIC_HW_DOWNLINKPACKET_H_
#define SANLACLASSIC_HW_DOWNLINKPACKET_H_

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

                // Todo fix this fucker.
                /*DownlinkPacket(SanlaPacket packet) {
                    message_id = packet.header.message_id;
                    memcpy(recipient_id, packet.header.recipient_id, sanla::messaging::RECIPIENT_ID_MAX_SIZE);
                    message_payload_length = packet.header.message_payload_length;
                    payload_chks = packet.header.payload_chks;                
                    std::string body_string(packet.body);
                    payloadBuffer.push_back(body_string);
                }*/
            };

        };
    };
};
#endif