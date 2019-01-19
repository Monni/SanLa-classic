#ifndef SANLACLASSIC_HW_DOWNLINKBUFFER_H_
#define SANLACLASSIC_HW_DOWNLINKBUFFER_H_

#include "hw/MessageBuffer.hpp"

namespace sanla {
    namespace hw_interfaces {
        namespace mq {
            class DownlinkBuffer:MessageBuffer {
                public:
                std::vector<SanlaPacket> packetBuffer;
                
                void onPacketReceived(SanlaPacket);

                virtual uint32_t GetBufferLength() override;
            };
        }; // mq
    }; // hw_interfaces
}; // sanla
#endif