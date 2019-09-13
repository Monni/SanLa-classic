#include "HardwareSerial.h" // TODO remove after debugging not needed.
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
using SanlaPacket = sanla::messaging::SanlaPacket;
using SanlaMessagePackage = sanla::messaging::SanlaMessagePackage;

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

/**
 * @brief Handle user input message.
 * Saves it to MessageStore and sends to UplinkBuffer.
 * 
 * @return true 
 * @return false 
 */
bool HandleMessage(SanlaMessagePackage&);

/**
 * @brief Handle incoming packet with REQ-flag.
 * Constructs a response packet and sends to UplinkBuffer.
 * 
 * @return true 
 * @return false 
 */
bool HandleResponse(SanlaPacket&);

bool ProcessPacket(SanlaPacket&);


private:
// Only Singleton of type SanlaProcessor is able to create or delete object of this class
SanlaProcessor(): m_dbuffer(this), m_ubuffer(), m_mstore(){};
~SanlaProcessor(){};

DownlinkBuffer m_dbuffer;
UplinkBuffer m_ubuffer;
MessageStore m_mstore;

};

using SanlaProcessorSingleton = Singleton<SanlaProcessor>;

}
#endif