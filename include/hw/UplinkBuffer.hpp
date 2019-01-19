#include "hw/MessageBuffer.hpp"

#ifndef SANLACLASSIC_HW_UPLINKBUFFER_H_
#define SANLACLASSIC_HW_UPLINKBUFFER_H_
namespace sanla {
    namespace hw_interfaces {
        namespace mq {
            class UplinkBuffer:MessageBuffer {
                public:
                std::vector<SanlaPacket> packetBuffer;
                bool send(SanlaPacket);
                PacketVector splitPackage(SanlaPackage);
            };
        }; // mq
    }; // hw_interfaces
}; // sanla
#endif