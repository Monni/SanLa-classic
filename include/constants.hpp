#ifndef SANLACLASSIC_CONSTANTS_H_
#define SANLACLASSIC_CONSTANTS_H_

#include <cstddef>

namespace sanla {

    namespace messaging {

        namespace sanlamessage {
            constexpr size_t MESSAGE_BODY_MAX_SIZE{2000};
        };

        namespace sanlapacket {
            constexpr size_t PACKET_MAX_SIZE{51};
            constexpr size_t PACKET_BODY_MAX_SIZE{39};
            constexpr size_t PACKET_HEADER_SIZE{12};
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