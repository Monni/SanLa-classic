#ifndef SANLACLASSIC_HW_UPLINKBUFFER_H_
#define SANLACLASSIC_HW_UPLINKBUFFER_H_

#include "HardwareSerial.h" // TODO remove after debugging not needed.
#include "hw/MessageBuffer.hpp"
#include "common/SanlaMessage.hpp"
#include "common/SanlaPacket.hpp"
#include "constants.hpp"

namespace sanla {
    namespace hw_interfaces {
        namespace mq {
            class UplinkBuffer:MessageBuffer {
                public:
                UplinkBuffer(){};
                ~UplinkBuffer(){};
                void send();

                /**
                 * @brief Add SanlaPacket into buffer.
                 * 
                 * @param packet SanlaPacket to be added
                 * @return true If packet was added
                 * @return false If packet wasn't added. This is caused by buffer already being full.
                 */
                bool addPacket(SanlaPacket);

                virtual uint32_t GetBufferLength() override;
                
                private:
                std::vector<SanlaPacket> packetBuffer;

                /**
                 * @brief Erases first SanlaPacket from UplinkBuffer.
                 * 
                 */
                void eraseFirstPacket();
            };
        }; // mq
    }; // hw_interfaces
}; // sanla
#endif