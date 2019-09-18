#include "common/SanlaProcessor.hpp"

namespace sanla {

bool SanlaProcessor::ProcessPacket(SanlaPacket &packet){
    switch (messaging::DefineActionToPacket(packet))
    {
    
    case messaging::ActionsE::RECEIVE: {
        Serial.println("messaging::ActionsE::RECEIVE");
        m_dbuffer.ReceivePacket(packet);
        return true;
    }

    case messaging::ActionsE::RESPOND: {
        Serial.println("messaging::ActionsE::RESPOND");
        HandleResponse(packet);
        //This packet has no further use
        (void)packet;
        return true;
    }
    
    case messaging::DROP: {
        Serial.println("messaging::ActionsE::DROP");
        // delete the packet
        (void)packet;
        return true;
    }
    default:
        return false;
    }
}

bool SanlaProcessor::HandleMessage(SanlaMessagePackage &message) {
    Serial.println("SanlaProcessor::HandleMessage");
    // Save message to MessageStore.
    m_mstore.Append(message);

    // Construct broadcast packets from message and push to uplinkbuffer.
    std::vector<SanlaPacket> packet_vector(messaging::buildBroadcastPacketsFromMessage(message));
    for (auto& packet : packet_vector) {
        m_ubuffer.addPacket(packet);
    }

    // Message has no use anymore.
    (void)message;

    return true;
}

bool SanlaProcessor::HandleResponse(SanlaPacket &input_packet) {
/*
    1. Get payload based on sequence.
    2. Construct packet.
    3. Push packet to UplinkBuffer.
*/
    auto message = m_mstore.GetMessage(input_packet.header.message_id);

    SanlaPacket packet = messaging::buildBroadcastPacketFromSequence(message, input_packet.header.payload_seq);
    return m_ubuffer.addPacket(packet);
}

void SanlaProcessor::SendUplinkBuffer() {
    m_ubuffer.send();
}

}