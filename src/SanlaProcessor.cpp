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

bool SanlaProcessor::HandleMessage(SanlaMessage &message) {
    Serial.println("SanlaProcessor::HandleMessage");
    // Save message to MessageStore.
    m_mstore.Put(message);

    // Notify interface of a message if this node is recipient.
    if (message.GetMessageHeader().recipient_id == 65535) { // TODO devices need to register into a group!
        interface.displayMessage(message);
    }

    // Construct broadcast packets from message and push to uplinkbuffer.
    std::vector<SanlaPacket> packet_vector(messaging::buildPacketsFromMessage(message));
    for (auto& packet : packet_vector) {
        HandlePacket(packet);
    }

    // Message has no use anymore.
    (void)message;

    return true;
}

bool SanlaProcessor::HandlePacket(SanlaPacket &packet) {
    
    // Stamp the packet with chip id from efuse before pushing it into UplinkBuffer.
    packet.header.sender_id = (SenderId_t)ESP.getEfuseMac();
    
    return m_ubuffer.addPacket(packet);
}

bool SanlaProcessor::messageExistsInStore(MessageId_t messageId) {
    return m_mstore.messageExists(messageId);
}

bool SanlaProcessor::HandleResponse(SanlaPacket &input_packet) {
    Serial.print("Received packet request for message ");
    Serial.print(input_packet.header.message_id);
    Serial.print(", sequence ");
    Serial.println(input_packet.header.payload_seq);

    SanlaPacket& packet = m_mstore.GetPacketBySequence(input_packet.header.message_id, input_packet.header.payload_seq);

    return HandlePacket(packet);
}

void SanlaProcessor::SendUplinkBuffer() {
    m_ubuffer.send();
}

void SanlaProcessor::notifyInterface(SanlaMessage& message) {
    interface.displayMessage(message);
}

SanlaInterface* SanlaProcessor::getInterfacePtr() {
    return &interface;
}

}