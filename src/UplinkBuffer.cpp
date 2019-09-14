#include "hw/UplinkBuffer.hpp"
#include "LoRaModule.hpp"

namespace sanla {
namespace hw_interfaces {
namespace mq {

void UplinkBuffer::send(){
    Serial.println("UplinkBuffer::send");
    while (!packetBuffer.empty() && sanla::lora::LoRaModule::sendPacket(packetBuffer.front())) {
        // Remove packet from buffer after it's been sent.
        UplinkBuffer::eraseFirstPacket();
    };
}

bool UplinkBuffer::addPacket(SanlaPacket packet){
    Serial.println("UplinkBuffer::addPacket");
    // Try to make more space into buffer before pushing anything new
    UplinkBuffer::send();

    if (packetBuffer.size() < UPLINKBUFFER_MAX_SIZE) {
        packetBuffer.push_back(packet);
        Serial.println("UplinkBuffer::addPacket::true");
        UplinkBuffer::send();
        return true;
    }
    Serial.println("UplinkBuffer::addPacket::false");
    return false;
}

uint32_t UplinkBuffer::GetBufferLength() {
    return packetBuffer.size();
}

void UplinkBuffer::eraseFirstPacket() {
    packetBuffer.erase(packetBuffer.begin());
}

} // mq
} // hw_interfaces
} // sanla