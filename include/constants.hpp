#ifndef SANLACLASSIC_CONSTANTS_H_
#define SANLACLASSIC_CONSTANTS_H_

#include <cstddef>

#include "common/typedefs.hpp"

namespace sanla {
    namespace lora {
        // Max lora packet size in bytes
        const size_t LORA_PACKAGE_SIZE{56};
        const size_t MAX_PACKET_PAYLOAD_SIZE{33};
    };

    namespace sanlamessage {
        constexpr size_t RECIPIENT_ID_MAX_SIZE{8};
        constexpr size_t PACKET_BODY_MAX_SIZE{20};
        constexpr size_t MESSAGE_BODY_SENDER_MAX_SIZE{12};
        constexpr size_t MESSAGE_BODY_PAYLOAD_MAX_SIZE{30};
        const Flag_t BRO {0x1}, REQ {0x2}, PRO {0x3}, PAC {0x4}, ACK {0x8},
        SEN {0xC}, RES {0xA}, PACREQ {0x6}, PACPRO {0x7}, PROACK {0xB},
        PACSEN {0xF}, PACRES {0xD};
    };
};
#endif