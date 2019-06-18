#include "hw/UplinkBuffer.hpp"
#include "LoRaModule.hpp"

namespace sanla {
namespace hw_interfaces {
namespace mq {

void UplinkBuffer::send(){
    while (!packetBuffer.empty() && sanla::lora::LoRaModule::sendPacket(packetBuffer.front())) {};
}

/**
 * @brief Add SanlaPacket into buffer.
 * 
 * @param packet SanlaPacket to be added
 * @return true If packet was added
 * @return false If packet wasn't added. This is caused by buffer already being full.
 */
bool UplinkBuffer::addPacket(SanlaPacket packet){
    // TODO flow: if there's space, push_back and send.
    // Otherwise try to send and then push_back.
    if (packetBuffer.size() < UPLINKBUFFER_MAX_SIZE) {
        packetBuffer.push_back(packet);
        UplinkBuffer::send(); // Todo temporarily here for testing.
        return true;
    }
    return false;
}

uint32_t UplinkBuffer::GetBufferLength() {
    return packetBuffer.size();
}

} // mq
} // hw_interfaces
} // sanla