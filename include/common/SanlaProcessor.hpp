#ifndef SANLACLASSIC_COMMON_SANLAPROCESSOR_H_
#define SANLACLASSIC_COMMON_SANLAPROCESSOR_H_

#include "Singleton.hpp"
#include "hw/DownlinkBuffer.hpp"
#include "hw/UplinkBuffer.hpp"
#include "hw/MessageStore.hpp"

using DownlinkBuffer = sanla::hw_interfaces::mq::DownlinkBuffer;
using UplinkBuffer = sanla::hw_interfaces::mq::UplinkBuffer;
using MessageStore = sanla::hw_interfaces::mq::MessageStore;

namespace sanla {

class SanlaProcessor {

private:
DownlinkBuffer m_dbuffer;
UplinkBuffer m_ubuffer;
MessageStore m_mstore;

};

using SanlaProcessorSingleton = Singleton<SanlaProcessor>;

}
#endif