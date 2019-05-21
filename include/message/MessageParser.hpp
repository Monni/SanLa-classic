#ifndef SANLACLASSIC_MESSAGE_MESSAGEPARSER_H_
#define SANLACLASSIC_MESSAGE_MESSAGEPARSER_H_

#include "common/SanlaMessage.hpp"
#include "common/SanlaPacket.hpp"

namespace sanla {
    namespace messaging {

        enum ActionsE {
            STORE,
            RESPOND,
            DROP
        };

        class MessageParser {
            public:
            MessageParser(){};
            ~MessageParser(){};

            uint32_t ParseMessage(SanlaPacket);

        };
    }; // messaging
}; // sanla
#endif