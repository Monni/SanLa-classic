#include <cstddef>

#ifndef SANLACLASSIC_CONSTANTS_H_
#define SANLACLASSIC_CONSTANTS_H_
namespace sanla {
    namespace lora {
        // Max lora packet size in bytes
        const std::size_t LORA_PACKAGE_SIZE{56};
    }
    namespace sanlamessage {
        constexpr size_t RECIPIENT_ID_MAX_SIZE{8};
        constexpr size_t PACKET_BODY_MAX_SIZE{20};
        constexpr size_t MESSAGE_BODY_SENDER_MAX_SIZE{12};
        constexpr size_t MESSAGE_BODY_PAYLOAD_MAX_SIZE{30};
    }
}
#endif