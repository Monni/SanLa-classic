#include "hw/MessageBuffer.hpp"

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