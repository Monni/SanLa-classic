#include "hw/UplinkBuffer.hpp"
#include "LoRaModule.hpp"

namespace sanla {
namespace hw_interfaces {
namespace mq {

void UplinkBuffer::send(){
    while (!packetBuffer.empty()) {
        if (sanla::lora::LoRaModule::sendPacket(packetBuffer.front())) {
            UplinkBuffer::eraseFirstPacket();
        } else {
            return;
        }
    };
}

bool UplinkBuffer::addPacket(SanlaPacket &packet){
    // First try to make more space into buffer.
    UplinkBuffer::send();

    if (packetBuffer.size() < UPLINKBUFFER_MAX_SIZE) {
        packetBuffer.push_back(packet);
        UplinkBuffer::send();
        return true;
    }
    return false;
}

uint32_t UplinkBuffer::GetBufferLength() {
    return packetBuffer.size();
}

void UplinkBuffer::eraseFirstPacket() {

    // Does this erase packet correctly?
    SanlaPacket& packet = *packetBuffer.begin();
    packetBuffer.erase(packetBuffer.begin());
    (void)packet;
}

} // mq
} // hw_interfaces
} // sanla