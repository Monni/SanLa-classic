#ifndef SANLACLASSIC_COMMON_UTILS_H_
#define SANLACLASSIC_COMMON_UTILS_H_

#include "common/SanlaMessage.hpp"
#include "common/SanlaPacket.hpp"
#include "hw/DownlinkPacket.hpp"

namespace sanla {
    namespace messaging {

        using DownlinkPacket = hw_interfaces::mq::DownlinkPacket;

        SanlaMessagePackage downlinkpacket_to_sanlamessage(DownlinkPacket);
        DownlinkPacket sanlapacket_to_downlinkpacket(SanlaPacket);

    }
}

#endif