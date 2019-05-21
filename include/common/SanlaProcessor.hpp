#include "Singleton.hpp"
#include "hw/DownlinkBuffer.hpp"
#include "hw/UplinkBuffer.hpp"
#include "hw/MessageStore.hpp"

#ifndef SANLACLASSIC_COMMON_SANLAPROCESSOR_H_
#define SANLACLASSIC_COMMON_SANLAPROCESSOR_H_

using DownlinkBuffer = sanla::hw_interfaces::mq::DownlinkBuffer;
using UplinkBuffer = sanla::hw_interfaces::mq::UplinkBuffer;
using MessageStore = sanla::hw_interfaces::mq::MessageStore;

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

private:
// Only Singleton of type SanlaProcessor is able to create or delete object of this class
SanlaProcessor(): m_dbuffer(), m_ubuffer(), m_mstore() {};
~SanlaProcessor(){};

DownlinkBuffer m_dbuffer;
UplinkBuffer m_ubuffer;
MessageStore m_mstore;

};

using SanlaProcessorSingleton = Singleton<SanlaProcessor>;

}
#endif