#include "common/SanlaProcessor.hpp"
#include "common/utils.hpp"

namespace sanla {

bool SanlaProcessor::ProcessPacket(SanlaPacket &packet){
    switch (messaging::DefineActionToPacket(packet))
    {
    
    case messaging::ActionsE::RECEIVE: {
        m_dbuffer.ReceivePacket(packet);
        return true;
    }

    case messaging::ActionsE::RESPOND: {
        HandleResponse(packet);
        //This packet has no further use
        (void)packet;
        return true;
    }
    
    case messaging::ActionsE::STORE: {
        // TODO STORE vain messageille. Tarvitaanko taalla?
        return true;
    }
    
    
    case messaging::DROP: {
        // delete the packet
        (void)packet;
        return true;
    }
    default:
        return false;
    }
}

bool SanlaProcessor::HandleMessage(SanlaMessagePackage &message) {
    // Save message to MessageStore.
    m_mstore.Append(message);

    // Construct broadcast packets from message and push to uplinkbuffer.
    std::vector<SanlaPacket> packet_vector(messaging::buildBroadcastPacketsFromMessage(message));
    for(std::size_t i = 0; i < packet_vector.size(); i++) {
        m_ubuffer.addPacket(packet_vector[i]);
    }
    return true;
}

bool SanlaProcessor::HandleResponse(SanlaPacket &input_packet) {
/*
    1. Get payload based on sequence.
    2. Construct packet.
    3. Push packet to UplinkBuffer.
*/
    auto message = m_mstore.GetMessage(input_packet.header.message_id);

    return m_ubuffer.addPacket(messaging::buildBroadcastPacketFromSequence(message, input_packet.header.payload_seq));
}

}