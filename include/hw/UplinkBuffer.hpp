#ifndef SANLACLASSIC_HW_UPLINKBUFFER_H_
#define SANLACLASSIC_HW_UPLINKBUFFER_H_

#include "hw/MessageBuffer.hpp"
#include "common/SanlaMessage.hpp"
#include "common/SanlaPacket.hpp"
#include "constants.hpp"

namespace sanla {
    namespace hw_interfaces {
        namespace mq {
            class UplinkBuffer:MessageBuffer {
                public:
                UplinkBuffer();
                ~UplinkBuffer();
                void send();
                bool addPacket(SanlaPacket);
                virtual uint32_t GetBufferLength() override;
                
                private:
                std::vector<SanlaPacket> packetBuffer;
            };
        }; // mq
    }; // hw_interfaces
}; // sanla
#endif