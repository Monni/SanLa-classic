#include "hw/MessageBuffer.hpp"

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