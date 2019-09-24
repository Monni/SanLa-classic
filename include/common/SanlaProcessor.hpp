#ifndef SANLACLASSIC_COMMON_SANLAPROCESSOR_H_
#define SANLACLASSIC_COMMON_SANLAPROCESSOR_H_

#include <Esp.h>
#include <HardwareSerial.h>
#include <Singleton.hpp>
#include "common/SanlaPacket.hpp"
#include "common/SanlaMessage.hpp"
#include "common/utils.hpp"
#include "hw/DownlinkBuffer.hpp"
#include "hw/UplinkBuffer.hpp"
#include "hw/MessageStore.hpp"
#include "message/MessageParser.hpp"
#include "ui/UserInterface.hpp"

using DownlinkBuffer = sanla::hw_interfaces::mq::DownlinkBuffer;
using UplinkBuffer = sanla::hw_interfaces::mq::UplinkBuffer;
using MessageStore = sanla::hw_interfaces::mq::MessageStore;
using SanlaPacket = sanla::messaging::SanlaPacket;
using SanlaMessage = sanla::messaging::SanlaMessage;
using SanlaInterface = sanla::ui::UserInterface;

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
bool HandleMessage(SanlaMessage&);

/**
 * @brief Forward given packet into UplinkBuffer.
 * 
 * @return true 
 * @return false 
 */
bool HandlePacket(SanlaPacket&);

/**
 * @brief Handle incoming packet with REQ-flag.
 * Constructs a response packet and sends to UplinkBuffer.
 * 
 * @return true 
 * @return false 
 */
bool HandleResponse(SanlaPacket&);

/**
 * @brief Try to send packets from UplinkBuffer.
 * 
 */
void SendUplinkBuffer();

bool ProcessPacket(SanlaPacket&);

/**
 * @brief Check message exists in store.
 * 
 * @return true 
 * @return false 
 */
bool messageExistsInStore(MessageId_t);

/**
 * @brief Get the Interface Ptr object for external usage.
 * 
 * @return SanlaInterface* 
 */
SanlaInterface* getInterfacePtr();


private:
// Only Singleton of type SanlaProcessor is able to create or delete object of this class
SanlaProcessor(): m_dbuffer(this), m_ubuffer(), m_mstore(), interface(this){};
~SanlaProcessor(){};

DownlinkBuffer m_dbuffer;
UplinkBuffer m_ubuffer;
MessageStore m_mstore;
SanlaInterface interface;

/**
 * @brief Send incoming Message into Interface.
 * 
 */
void notifyInterface(SanlaMessage&);

};

using SanlaProcessorSingleton = Singleton<SanlaProcessor>;

}
#endif