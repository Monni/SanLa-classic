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

SanlaMessage MessageStore::Get(MessageId_t messageId) {
    auto message = m_store[messageId]; // TODO catch exceptions raised here and inform method caller

    // TODO MessageStore should probably return a copy..
    return *message;
}

void MessageStore::Put(SanlaMessage &msg)
{
    m_store.insert({msg.GetMessageHeader().message_id, new SanlaMessage(msg)});
}

bool MessageStore::messageExists(MessageId_t messageId) {
    if ( m_store.find(messageId) == m_store.end() ) {
        return false;
    }
    return true;
}

SanlaPacket MessageStore::GetPacketBySequence(MessageId_t message_id, PayloadSeq_t payloadSequence) {
    SanlaMessage *message = m_store[message_id]; // TODO catch exceptions raised here and inform method caller

    std::string messageBody(message->GetMessageBody());
    std::string payloadString(messageBody.substr(payloadSequence, sanla::messaging::sanlapacket::PACKET_BODY_MAX_SIZE-1));

    char payloadArr[messaging::sanlapacket::PACKET_BODY_MAX_SIZE];
    strcpy(payloadArr, payloadString.c_str());

    SanlaPacket packet{};
    packet.copy_headers_from_message(message->GetMessageHeader(), payloadArr);
    packet.header.payload_seq = payloadSequence;
    strcpy(packet.body, payloadArr);

    if (payloadSequence + sanla::messaging::sanlapacket::PACKET_BODY_MAX_SIZE-1 >= messageBody.length()) {
        packet.header.flags = messaging::END;
    }

    return packet;
}

} // mq
} // hw_interfaces
} // sanla