#ifndef SANLACLASSIC_HW_MESSAGESTORE_H_
#define SANLACLASSIC_HW_MESSAGESTORE_H_

#include <map>
#include "common/SanlaMessage.hpp"
#include "hw/MessageBuffer.hpp"

namespace sanla {
    namespace hw_interfaces {
        namespace mq {
            // TODO: Add custom allocator which uses Flash memory
            using MessageMap = std::map<PackageId_t, SanlaPackage/*,FlashAllocator<SanlaPackage>*/>;

            class MessageStore:MessageBuffer {
                public:                

                virtual uint32_t GetBufferLength() override;

                void Append(SanlaPackage);
                SanlaPackage Pop(PackageId_t);
                SanlaPackage Get(PackageId_t);

                PacketVector GetPackets(SanlaPackage);
                SanlaPacket GetNthPacket(SanlaPackage, size_t);

                private:
                PacketVector splitPackage(SanlaPackage);
                MessageMap m_store;
            };
        }; // mq
    }; // hw_interfaces
}; // sanla
#endif