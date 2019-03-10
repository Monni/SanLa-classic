#ifndef SANLACLASSIC_HW_MESSAGEBUFFER_H_
#define SANLACLASSIC_HW_MESSAGEBUFFER_H_

#include <vector>
#include "common/SanlaMessage.hpp"
#include "common/SanlaPacket.hpp"

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

            };
        }; // mq
    }; // hw_interfaces
}; // sanla
#endif