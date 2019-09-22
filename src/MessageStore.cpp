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

void MessageStore::Append(SanlaPackage &pkg)
{
    m_store.insert({pkg.GetPackageHeader().message_id, new SanlaPackage(pkg)});
}

SanlaPacket MessageStore::GetPackagePart(MessageId_t message_id, size_t payloadStartPos) {
    auto package = m_store[message_id]; // TODO catch exceptions raised here and inform method caller
    std::string msg(package->GetPackageBody());
    auto new_payload = msg.substr(payloadStartPos, sanla::messaging::sanlapacket::PACKET_BODY_MAX_SIZE).c_str();
    SanlaPacket output{};
    output.copy_headers_from_message(package->GetPackageHeader(), package->GetPackageBody());
    strcpy(output.body, new_payload); // Possible segfault due to missing Null terminator
    return output;

}

SanlaPackage MessageStore::GetMessage(MessageId_t messageId) {
    auto message = m_store[messageId]; // TODO catch exceptions raised here and inform method caller

    // TODO MessageStore should probably return a copy..
    return *message;
}

bool MessageStore::messageExists(MessageId_t messageId) {
    if ( m_store.find(messageId) == m_store.end() ) {
        return false;
    }
    return true;
}

} // mq
} // hw_interfaces
} // sanla