#include <array>
#include <string.h>
#include <math.h>
#include "constants.hpp"
#include "hw/MessageStore.hpp"

namespace sanla {
namespace hw_interfaces {
namespace mq {

SanlaPacket MessageStore::GetPackagePart(PackageId_t packageId, size_t payloadStartPos) {
    auto package = m_store[packageId]; // TODO catch exceptions raised here and inform method caller
    std::string msg(package->GetPackageBody().payload);
    auto new_payload = msg.substr(payloadStartPos, sanla::lora::MAX_PACKET_PAYLOAD_SIZE).c_str();
    SanlaPacket output{};
    output.copy_headers_from_message(package->GetPackageHeader(), package->GetPackageBody());
    strncpy(output.body.payload, new_payload, sanla::lora::MAX_PACKET_PAYLOAD_SIZE); // Possible segfault due to missing Null terminator
    return output;

}
} // mq
} // hw_interfaces
} // sanla