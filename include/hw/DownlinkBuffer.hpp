#ifndef SANLACLASSIC_HW_DOWNLINKBUFFER_H_
#define SANLACLASSIC_HW_DOWNLINKBUFFER_H_

#include <map>
#include "hw/MessageBuffer.hpp"
#include "message/MessageParser.hpp"
#include "DownlinkPacket.hpp"

namespace sanla {
    namespace hw_interfaces {
        namespace mq {
            using DownlinkPacketMap = std::map<MessageId_t, DownlinkPacket*>;

            class DownlinkBuffer:MessageBuffer {
                public:
                DownlinkBuffer(){};
                DownlinkBuffer( void *processor_ptr) : m_sanla_processor_ptr(processor_ptr) {};
                ~DownlinkBuffer(){};
                void ReceivePacket(SanlaPacket&);

                virtual uint32_t GetBufferLength() override;

                private:
                DownlinkPacketMap downlinkPacketBuffer;

                /**
                 * @brief Validate incoming packet is valid.
                 * 
                 * @return true 
                 * @return false 
                 */
                bool validatePacket(SanlaPacket&);

                /**
                 * @brief Validate stored DownlinkPacket is ready to be parsed into
                 * a message and to be sent into MessageStore.
                 * 
                 * @return true if message is ready.
                 * @return false if message is not ready.
                 */
                bool validateMessageReady(DownlinkPacket&);
                
                bool StorePacket(SanlaPacket&);

                void SendMessageToStore(SanlaPackage&);

                // This is later casted to SanlaProcessor
                void* m_sanla_processor_ptr = nullptr;

            };
        }; // mq
    }; // hw_interfaces
}; // sanla
#endif