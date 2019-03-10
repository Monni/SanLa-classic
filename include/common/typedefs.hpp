#ifndef SANLACLASSIC_COMMON_TYPEDEFS_H_
#define SANLACLASSIC_COMMON_TYPEDEFS_H_

#include <stdint.h>
#include <cstddef>
#include <string>

namespace sanla
{
    using PackageId_t = uint32_t;
    using PayloadSeq_t = uint8_t;
    using PayloadLength_t = uint16_t;
    using SenderId_t = uint16_t;
    using RecipientId_t = uint16_t;
    using PayloadChecksum_t = uint32_t;
    using Flag_t = u_char;
}; // sanla
#endif