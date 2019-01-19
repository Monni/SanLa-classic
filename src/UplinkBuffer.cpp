#include <string.h>
#include "hw/UplinkBuffer.hpp"
#include "constants.hpp"
#include "common/SanlaMessage.hpp"

using namespace sanla::hw_interfaces::mq;

bool UplinkBuffer::send(SanlaPacket packet){
    return false;
}

/*
TODO: Actuall split implementation logic
This method should split SanlaPackage to SanlaPackets of size 
sanla::lora::LORA_PACKAGE_SIZE
*/
PacketVector UplinkBuffer::splitPackage(SanlaPackage package){
    const size_t package_size = sizeof package;
    const uint32_t no_packets =  package_size / sanla::lora::LORA_PACKAGE_SIZE;
    const size_t payload_length = strlen(package.GetPackageBody().payload);
    const auto package_h = package.GetPackageHeader();
    PacketVector output{};
    output.reserve(no_packets);
    char recipient_id[sanla::sanlamessage::RECIPIENT_ID_MAX_SIZE];
    strcpy(recipient_id, package_h.recipient_id.c_str());
    for (uint32_t i = 0; i <= no_packets; i++) {
        SanlaPacket packet{};
        packet.header.flags = sanla::sanlamessage::PACSEN;
        packet.header.package_id = package_h.package_id;
        packet.header.sender_id = package_h.sender_id;
        strcpy(packet.header.recipient_id, recipient_id);
        packet.header.payload_seq = i;
        packet.header.payload_chks = 1449; // TODO: replace this with a function calculating a hash of payload
        strcpy(packet.header.recipient_id, package.GetPackageBody().payload); // TODO: replace this with a function splitting payload
        output.push_back(packet);
    }

    return output;
}