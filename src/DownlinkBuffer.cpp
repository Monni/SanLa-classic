#include "hw/DownlinkBuffer.hpp"
#include "constants.hpp"

namespace sanla {
    namespace hw_interfaces {
        namespace mq {

            void DownlinkBuffer::onPacketReceived(SanlaPacket packet) {
                auto result = parser.ParseMessage(packet);
                switch (result)
                {
                    case sanlamessage::STORE:
                        StorePacket(packet);
                        break;
                    
                    case sanlamessage::DROP:
                        DropPacket(packet);
                        break;
                    
                    case sanlamessage::RESPOND:
                        RespondPacket(packet);
                        break;
                
                    default:
                        break;
                }
            }

            uint32_t DownlinkBuffer::GetBufferLength() {
                return packetBuffer.size();
            }

            void DownlinkBuffer::DropPacket(SanlaPacket packet){
                (void)packet;
            }

            bool DownlinkBuffer::StorePacket(SanlaPacket packet) {
                if (packetBuffer.size() >= DOWNLINKBUFFER_MAX_SIZE) {
                    DropPacket(packet);
                    return false;
                }
                packetBuffer.push_back(packet);
                return true;
            }

            void DownlinkBuffer::RespondPacket(SanlaPacket packet) {
                //TODO formulate response and push it to object which manages both uplink and downlink buffers
            }
        }
    }
}