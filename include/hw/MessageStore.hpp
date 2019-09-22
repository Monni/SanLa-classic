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

                SanlaMessage Pop(MessageId_t);
                SanlaMessage Get(MessageId_t);
                void Append(SanlaMessage&);

                SanlaPacket GetNthPacket(SanlaMessage, size_t);
                SanlaPacket GetPackagePart(MessageId_t, size_t);
                SanlaMessage GetMessage(MessageId_t);

                /**
                 * @brief Check if a message exists in store.
                 * 
                 * @return true 
                 * @return false 
                 */
                bool messageExists(MessageId_t);

                private:
                MessageMap m_store;
            };
        }; // mq
    }; // hw_interfaces
}; // sanla
#endif