#ifndef SANLACLASSIC_CONSTANTS_H_
#define SANLACLASSIC_CONSTANTS_H_

#include <cstddef>

namespace sanla {
    namespace lora {
        // Max lora packet size in bytes
        const size_t LORA_PACKAGE_SIZE{56};
    };

    namespace sanlamessage { // TODO name to general "messaging" and add sub-namespaces for sanlamessage and sanlapacket?
        constexpr size_t RECIPIENT_ID_MAX_SIZE{8};
        constexpr size_t MESSAGE_BODY_SENDER_MAX_SIZE{12};
        constexpr size_t MESSAGE_BODY_PAYLOAD_MAX_SIZE{30};

        namespace sanlapacket {
            constexpr size_t PACKET_BODY_MAX_SIZE{20};
        };
    };

    namespace hw_interfaces {
        namespace mq {
            constexpr size_t UPLINKBUFFER_MAX_SIZE{50};
            constexpr size_t DOWNLINKBUFFER_MAX_SIZE{50};
        };
    };
};
#endif