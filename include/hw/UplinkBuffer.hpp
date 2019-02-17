#ifndef SANLACLASSIC_HW_UPLINKBUFFER_H_
#define SANLACLASSIC_HW_UPLINKBUFFER_H_

#include "hw/MessageBuffer.hpp"

namespace sanla {
    namespace hw_interfaces {
        namespace mq {
            class UplinkBuffer:MessageBuffer {
                public:
                std::vector<SanlaPacket> packetBuffer;
                
                bool send(SanlaPacket);
                
                virtual uint32_t GetBufferLength() override;
            };
        }; // mq
    }; // hw_interfaces
}; // sanla
#endif