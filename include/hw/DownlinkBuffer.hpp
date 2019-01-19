#include "hw/MessageBuffer.hpp"

#ifndef SANLACLASSIC_HW_DOWNLINKBUFFER_H_
#define SANLACLASSIC_HW_DOWNLINKBUFFER_H_

namespace sanla {
    namespace hw_interfaces {
        namespace mq {
            class DownlinkBuffer:MessageBuffer {
                public:
                std::vector<SanlaPacket> packetBuffer;
                void onPacketReceived(SanlaPacket);
            };
        }; // mq
    }; // hw_interfaces
}; // sanla
#endif