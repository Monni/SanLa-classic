#include "Singleton.hpp"
#include "common/SanlaPacket.hpp"
#include "common/SanlaMessage.hpp"
#include "hw/DownlinkBuffer.hpp"
#include "hw/UplinkBuffer.hpp"
#include "hw/MessageStore.hpp"
#include "message/MessageParser.hpp"

#ifndef SANLACLASSIC_COMMON_SANLAPROCESSOR_H_
#define SANLACLASSIC_COMMON_SANLAPROCESSOR_H_

using DownlinkBuffer = sanla::hw_interfaces::mq::DownlinkBuffer;
using UplinkBuffer = sanla::hw_interfaces::mq::UplinkBuffer;
using MessageStore = sanla::hw_interfaces::mq::MessageStore;
using SanlaPacket = sanla::sanlamessage::SanlaPacket;
using MessageParser = sanla::sanlamessage::MessageParser;

namespace sanla {

class SanlaProcessor {

friend Singleton<SanlaProcessor>;

public:
// This class is not moveable
SanlaProcessor(SanlaProcessor&&) = delete;
SanlaProcessor& operator=(SanlaProcessor&&) = delete;
// This class is not copyable
SanlaProcessor(const SanlaProcessor&) = delete;
SanlaProcessor& operator=(const SanlaProcessor&) = delete;

bool HandleUplinkMessage(SanlaMessagePackage &messagePackage)
{
    // should we put packets to Uplink buffer here?
    m_mstore.Append(messagePackage);
    return true;
};

bool HandleDownlinkMessage_cb(SanlaMessagePackage){
    //Downlink buffer accepts only SanlaPackets
    //What do we do here?
    return false;
};
bool HandleResponseForPacket_cb(SanlaPacket &packet){
    auto action = m_parser.ParseMessage(packet);
    switch (action)
    {
    case sanlamessage::ActionsE::STORE:
        // where did we store this again?
        break;
    
    case sanlamessage::ActionsE::RESPOND:
        // Formulate a response here
        break;
    
    case sanlamessage::ActionsE::DROP:
        // delete the packet
        (void)packet;
        break;
    
    default:
        break;
    }
    return false;
};

private:
// Only Singleton of type SanlaProcessor is able to create or delete object of this class
SanlaProcessor(): m_dbuffer(), m_ubuffer(), m_mstore(), m_parser(){};
~SanlaProcessor(){};

DownlinkBuffer m_dbuffer;
UplinkBuffer m_ubuffer;
MessageStore m_mstore;
MessageParser m_parser;

};

using SanlaProcessorSingleton = Singleton<SanlaProcessor>;

}
#endif