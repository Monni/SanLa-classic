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

        /**
         *  @brief Constructs a SanlaPacket from SanlaMessagePackage.
         *  
         *  @return SanlaPacket
         */
        SanlaPacket buildBroadcastPacket(SanlaMessagePackage&);

        /**
         * @brief Constructs a host-to-network byte order char array of a SanlaPacket.
         * 
         */
        void htonSanlaPacket(SanlaPacket, sanlapacket::SerializedPacket_t);

        /**
         * @brief Constructs a host-to-network byte order char array of a SanlaPacket header.
         * 
         */
        void htonSanlaPacketHeader(SanlaPacketHeader, sanlapacket::SerializedPacketHeader_t);
        
        /**
         * @brief Constructs a SanlaPacket from network-to-host char array.
         * 
         * @return SanlaPacket Constructed SanlaPacket.
         */
        SanlaPacket ntohSanlaPacket(sanlapacket::SerializedPacket_t);

        /**
         * @brief Constructs a SanlaPacket header from network-to-host char array.
         * 
         * @return SanlaPacketHeader Constucted header.
         */
        SanlaPacketHeader ntohSanlaPacketHeader(sanlapacket::SerializedPacketHeader_t);
    }
}

#endif