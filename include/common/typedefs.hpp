#ifndef SANLACLASSIC_COMMON_TYPEDEFS_H_
#define SANLACLASSIC_COMMON_TYPEDEFS_H_

#include <map>
#include <stdint.h>
#include <cstddef>
#include <string>
#include "constants.hpp"

namespace sanla
{
    using SenderId_t = uint16_t;
    using RecipientId_t = uint16_t;

    using Flags_t = u_char;
    using PayloadLength_t = uint8_t;
    using PayloadChecksum_t = uint16_t;

    using MessageId_t = uint16_t;
    using PayloadSeq_t = uint16_t;
    
    
    namespace messaging {

        namespace sanlapacket {
            using SerializedPacket_t = char[PACKET_MAX_SIZE];
            using SerializedPacketHeader_t = char[PACKET_HEADER_SIZE];
            using Payload_t = char[PACKET_BODY_MAX_SIZE];
        };

        namespace sanlamessage {
            using Payload_t = char[MESSAGE_BODY_MAX_SIZE];
        };

        namespace dl_sanlapacket {
            using PayloadBuffer_t = std::map<std::pair<PayloadSeq_t, Flags_t>, std::string>;
        }
        
    }; // messaging
}; // sanla
#endif