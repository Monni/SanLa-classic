#include "message/MessageParser.hpp"

namespace sanla {
    namespace messaging {

        ActionsE DefineActionToPacket(const SanlaPacket &packet){
            switch(packet.header.flags){

                case messaging::REQ:
                    return RESPOND;

                default:
                    return RECEIVE;
            }
        }

    } // messaging
} // sanla