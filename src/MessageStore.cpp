#include <array>
#include <string.h>
#include <math.h>
#include "constants.hpp"
#include "hw/MessageStore.hpp"

namespace sanla {
namespace hw_interfaces {
namespace mq {

uint32_t MessageStore::GetBufferLength(){
    return m_store.size();
}

SanlaPacket MessageStore::GetPackagePart(MessageId_t packageId, size_t payloadStartPos) {
    auto package = m_store[packageId]; // TODO catch exceptions raised here and inform method caller
    std::string msg(package->GetPackageBody());
    auto new_payload = msg.substr(payloadStartPos, sanla::messaging::sanlapacket::PACKET_BODY_MAX_SIZE).c_str();
    SanlaPacket output{};
    output.copy_headers_from_message(package->GetPackageHeader(), package->GetPackageBody());
    strcpy(output.body, new_payload); // Possible segfault due to missing Null terminator
    return output;

}
} // mq
} // hw_interfaces
} // sanla