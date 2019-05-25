#include "common/SanlaProcessor.hpp"

namespace sanla {

bool SanlaProcessor::HandleUplinkMessage(SanlaMessagePackage &messagePackage)
{
    // should we put packets to Uplink buffer here?
    m_mstore.Append(messagePackage);
    return true;
}

bool SanlaProcessor::ProcessPacket(SanlaPacket &packet){
    switch (messaging::DefineActionToPacket(packet))
    {
    case messaging::ActionsE::STORE: {
        //Somehow check if the packet is already in buffer
        if(true){
            m_dbuffer.ReceivePacket(packet);
        }
        else{
            // We drop the packet
            (void)packet;
        }
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
    //TODO: Implement this
    return inputPacket;
}
}