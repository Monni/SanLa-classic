#ifndef SANLACLASSIC_MESSAGE_MESSAGEPARSER_H_
#define SANLACLASSIC_MESSAGE_MESSAGEPARSER_H_

#include "common/SanlaMessage.hpp"
#include "common/SanlaPacket.hpp"

namespace sanla {
    namespace messaging {

        enum ActionsE {
            RECEIVE,
            RESPOND,
            DROP
        };
// This doesn't need to be a class. I think this should be refactored as a function
        ActionsE DefineActionToPacket(const SanlaPacket&);
    }; // messaging
}; // sanla
#endif