#include "hw/UplinkBuffer.hpp"

namespace sanla {
namespace hw_interfaces {
namespace mq {

void UplinkBuffer::send(){
    while (!packetBuffer.empty()) {
        /*
        TODO: LoRaModule needs to be accessed before this can be used!
        */

        /*if (!LoRa.beginPacket()) {
            break;
        }
        char serialized_packet[41]{};
        //sanla::sanlamessage::htonSanlaPacket(packetBuffer.front(), serialized_packet);
        LoRa.write((uint8_t*)serialized_packet, 41);
        LoRa.endPacket();

        // Revert back to listening mode
        LoRa.receive();*/
    }
}

/**
 * @brief Add SanlaPacket into buffer.
 * 
 * @param packet SanlaPacket to be added
 * @return true If packet was added
 * @return false If packet wasn't added. This is caused by buffer already being full.
 */
bool UplinkBuffer::addPacket(SanlaPacket packet){
    if (packetBuffer.size() < UPLINKBUFFER_MAX_SIZE) {
        packetBuffer.push_back(packet);
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