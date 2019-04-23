#ifndef SANLACLASSIC_HW_DOWNLINKBUFFER_H_
#define SANLACLASSIC_HW_DOWNLINKBUFFER_H_

#include <map>
#include "hw/MessageBuffer.hpp"
#include "message/MessageParser.hpp"
#include "DownlinkPacket.hpp"

namespace sanla {
    namespace hw_interfaces {
        namespace mq {
            using MessageParser = sanlamessage::MessageParser;
            using DownlinkPacketMap = std::map<PackageId_t, DownlinkPacket*>;

            class DownlinkBuffer:MessageBuffer {
                public:
                DownlinkBuffer();
                void onPacketReceived(SanlaPacket);

                virtual uint32_t GetBufferLength() override;

                private:
                DownlinkPacketMap downlinkPacketBuffer;
                MessageParser parser;

                /**
                 * @brief Parse SanlaPacket into smaller format for storing incoming packets.
                 * 
                 * @return DownlinkPacket for storage.
                 */
                DownlinkPacket PacketToDownlinkPacket(SanlaPacket);
                
                void DropPacket(SanlaPacket);
                bool StorePacket(SanlaPacket);
                void RespondPacket(SanlaPacket);
            };
        }; // mq
    }; // hw_interfaces
}; // sanla
#endif