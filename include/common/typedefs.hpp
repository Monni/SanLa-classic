#ifndef SANLACLASSIC_COMMON_TYPEDEFS_H_
#define SANLACLASSIC_COMMON_TYPEDEFS_H_

#include <stdint.h>
#include <cstddef>
#include <string>
#include "constants.hpp"

namespace sanla
{
    using PackageId_t = uint32_t;
    using PayloadSeq_t = uint8_t;
    using PayloadLength_t = uint16_t;
    using SenderId_t = uint16_t;
    using RecipientId_t = char[sanlamessage::RECIPIENT_ID_MAX_SIZE];
    using PayloadChecksum_t = uint32_t;
    using Flag_t = u_char;
}; // sanla
#endif