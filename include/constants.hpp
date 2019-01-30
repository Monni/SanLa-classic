#ifndef SANLACLASSIC_CONSTANTS_H_
#define SANLACLASSIC_CONSTANTS_H_

#include <cstddef>

namespace sanla {
    namespace lora {
        // Max lora packet size in bytes
        const std::size_t LORA_PACKAGE_SIZE{56};
        const size_t MAX_PACKET_PAYLOAD_SIZE{33};
    }
}
#endif