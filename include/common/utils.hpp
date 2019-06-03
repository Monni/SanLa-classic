#ifndef SANLACLASSIC_COMMON_UTILS_H_
#define SANLACLASSIC_COMMON_UTILS_H_

#include "common/SanlaMessage.hpp"
#include "common/SanlaPacket.hpp"
#include "hw/DownlinkPacket.hpp"

namespace sanla {
    namespace messaging {

        using DownlinkPacket = hw_interfaces::mq::DownlinkPacket;

        /**
         * @brief Construct a SanlaMessage from DownlinkPacket.
         * 
         * @return SanlaMessagePackage 
         */
        SanlaMessagePackage downlinkpacketToSanlamessage(DownlinkPacket);

        /**
         * @brief Construct a DownlinkPacket from SanlaPacket.
         * 
         * @return DownlinkPacket 
         */
        DownlinkPacket sanlapacketToDownlinkpacket(SanlaPacket);

    }
}

#endif