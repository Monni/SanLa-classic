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

                virtual size_t GetBufferLength() override;

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

                /**
                 * @brief Build and send a request packet to replace malformed or missing packet.
                 * 
                 */
                void requestMissingPacket(MessageId_t, PayloadSeq_t);

                /**
                 * @brief Check if message already exists in MessageStore in which incoming packet is belonging to.
                 * 
                 * @return true 
                 * @return false 
                 */
                bool messageExistsInStore(MessageId_t);

                // This is later casted to SanlaProcessor
                void* m_sanla_processor_ptr = nullptr;

            };
        }; // mq
    }; // hw_interfaces
}; // sanla
#endif