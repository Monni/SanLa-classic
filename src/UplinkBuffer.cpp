#include "hw/UplinkBuffer.hpp"
#include "constants.hpp"

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
    size_t packet_size = sizeof package;
    PacketVector output{};

    return output;
}