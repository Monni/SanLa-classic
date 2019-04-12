#ifndef SANLACLASSIC_HW_DOWNLINKBUFFER_H_
#define SANLACLASSIC_HW_DOWNLINKBUFFER_H_

#include "hw/MessageBuffer.hpp"
#include "message/MessageParser.hpp"

namespace sanla {
    namespace hw_interfaces {
        namespace mq {
            using MessageParser = sanlamessage::MessageParser;

            class DownlinkBuffer:MessageBuffer {
                public:
                DownlinkBuffer();
                void onPacketReceived(SanlaPacket);

                virtual uint32_t GetBufferLength() override;

                private:

                std::vector<SanlaPacket> packetBuffer;
                MessageParser parser;
                
                void DropPacket(SanlaPacket);
                bool StorePacket(SanlaPacket);
                void RespondPacket(SanlaPacket);
            };
        }; // mq
    }; // hw_interfaces
}; // sanla
#endif