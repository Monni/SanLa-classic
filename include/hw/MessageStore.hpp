#include <map>
#include "common/SanlaMessage.hpp"
#include "hw/MessageBuffer.hpp"
#include "common/typedefs.hpp"
#include "constants.hpp"

#ifndef SANLACLASSIC_HW_MESSAGESTORE_H_
#define SANLACLASSIC_HW_MESSAGESTORE_H_

namespace sanla {
    namespace hw_interfaces {
        namespace mq {
            // TODO: Add custom allocator which uses Flash memory
            using MessageMap = std::map<MessageId_t, SanlaMessage* /*,FlashAllocator<SanlaMessage>*/>;

            class MessageStore:MessageBuffer {
                public:

                MessageStore(){};
                ~MessageStore(){};
                virtual uint32_t GetBufferLength() override;

                SanlaMessage Pop(MessageId_t); // TODO not yet implemented. Is this needed?

                /**
                 * @brief Gets a message from MessageStore.
                 * 
                 * @return SanlaMessage 
                 */
                SanlaMessage Get(MessageId_t);

                /**
                 * @brief Puts a new message into MessageStore.
                 * 
                 */
                void Put(SanlaMessage&);

                /**
                 * @brief Check if a message exists in store.
                 * 
                 * @return true 
                 * @return false 
                 */
                bool messageExists(MessageId_t);

                /**
                 * @brief Get single Packet from Message based on given payload sequence.
                 * 
                 * @return SanlaPacket 
                 */
                SanlaPacket& GetPacketBySequence(MessageId_t, PayloadSeq_t);

                private:
                MessageMap m_store;
            };
        }; // mq
    }; // hw_interfaces
}; // sanla
#endif