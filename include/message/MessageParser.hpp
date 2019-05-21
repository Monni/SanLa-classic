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
// This doesn't need tp be a class. I think this should be refactored as a function
        class MessageParser {
            public:
            MessageParser(){};
            ~MessageParser(){};

            uint32_t ParseMessage(SanlaPacket&);

        };
    }; // messaging
}; // sanla
#endif