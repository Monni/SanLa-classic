#include <vector>
#include "common/SanlaMessage.hpp"

namespace sanla {
    namespace hw_interfaces {
        namespace mq {
            using SanlaPacket = sanla::sanlamessage::SanlaPacket;
            using SanlaPackage = sanla::SanlaMessagePackage;
            using PacketVector = std::vector<SanlaPacket>;

            class MessageBuffer {
                public:
                MessageBuffer();
                ~MessageBuffer();

                virtual uint32_t GetBufferLength();

                std::vector<SanlaPackage> packageQueue;
            };
        }; // mq
    }; // hw_interfaces
}; // sanla