#include "common/SanlaProcessor.hpp"
#include "common/utils.hpp"

namespace sanla {

bool SanlaProcessor::HandleUplinkMessage(SanlaMessagePackage &sanla_message)
{
    // Save message to MessageStore.
    m_mstore.Append(sanla_message);

    // Construct broadcast packets from message and push all to uplinkbuffer.
    std::vector<SanlaPacket> packet_vector(messaging::buildBroadcastPacketsFromMessage(sanla_message));
    for(std::size_t i = 0; i < packet_vector.size(); i++) {
        m_ubuffer.addPacket(packet_vector[i]);
    }
    return true;
}

bool SanlaProcessor::ProcessPacket(SanlaPacket &packet){
    switch (messaging::DefineActionToPacket(packet))
    {
    
    case messaging::ActionsE::STORE: {
        m_dbuffer.ReceivePacket(packet);
        return true;
    }
    
    case messaging::ActionsE::RESPOND: {
        m_ubuffer.addPacket(BuildResponseToPacket(packet));
        //This packet has no further use
        (void)packet;
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

void SanlaProcessor::StoreCompleteMessage(SanlaMessagePackage &package) {
    m_mstore.Append(package);
}

SanlaPacket SanlaProcessor::BuildResponseToPacket(const SanlaPacket &inputPacket){
    //TODO: Implement this here or into utils?
    /*
        1. Check for flag. Done in somewhere else??
        2. Get payload based on sequence.
        3. Construct packet.
        4. Push packet to uplinkbuffer.
    */
    return inputPacket;
}
}