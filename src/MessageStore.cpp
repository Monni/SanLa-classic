#include <array>
#include <string.h>
#include <math.h>
#include "constants.hpp"
#include "hw/MessageStore.hpp"
#include "common/SanlaMessage.hpp"

namespace sanla {
namespace hw_interfaces {
namespace mq {

using PayloadVector = std::vector<payload_t>;

PayloadVector splitPayload(const char* payload) {
    const size_t max = lora::MAX_PACKET_PAYLOAD_SIZE;
    PayloadVector output;
    size_t total_length = strlen(payload);
    size_t no_split_payloads = static_cast<size_t>(ceilf(
        static_cast<float>(total_length) / static_cast<float>(max)
    ));
    output.reserve(no_split_payloads);
    for (size_t i=0; i <= no_split_payloads; i++){
        memcpy(output[i], payload+(i * max - 1), max);
    }

    return output;
}

PacketVector MessageStore::splitPackage(SanlaPackage package){
    const size_t package_size = sizeof package;
    const size_t no_packets =  package_size / sanla::lora::LORA_PACKAGE_SIZE;
    const size_t payload_length = strlen(package.GetPackageBody().payload);
    const auto package_h = package.GetPackageHeader();
    PacketVector output{};
    output.reserve(no_packets);
    PayloadVector payloads = splitPayload(package.GetPackageBody().payload);
    char recipient_id[sanla::sanlamessage::RECIPIENT_ID_MAX_SIZE];
    strcpy(recipient_id, package_h.recipient_id.c_str());

    for (size_t i = 0; i <= no_packets; i++) {
        SanlaPacket packet{};
        packet.flags = sanla::sanlamessage::PACSEN;
        packet.package_id = package_h.package_id;
        packet.sender_id = package_h.sender_id;
        strcpy(packet.recipient_id, recipient_id);
        packet.payload_seq = i;
        packet.payload_chks = 1449; // TODO: replace this with a function calculating a hash of payload
        strcpy(packet.payload, payloads[i]);
        output.push_back(packet);
    }

    return output;
}
} // mq
} // hw_interfaces
} // sanla