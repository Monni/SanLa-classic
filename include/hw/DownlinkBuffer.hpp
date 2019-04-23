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
            using DownlinkPacketMap = std::map<MessageId_t, DownlinkPacket*>;

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

                /**
                 * @brief Validate stored DownlinkPacket is ready to be parsed into
                 * a message and to be sent into MessageStore.
                 * 
                 * @return true if message is ready.
                 * @return false if message is not ready.
                 */
                bool validateMessageReady(DownlinkPacket);
                
                void DropPacket(SanlaPacket);
                bool StorePacket(SanlaPacket);
                void RespondPacket(SanlaPacket);
            };
        }; // mq
    }; // hw_interfaces
}; // sanla
#endif