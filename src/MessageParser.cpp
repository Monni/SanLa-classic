#include "message/MessageParser.hpp"

namespace sanla {
    namespace messaging {

        ActionsE DefineActionToPacket(const SanlaPacket &packet){

            switch(packet.header.flags){
                case messaging::BRO:
                case messaging::END:
                    return RECEIVE;                

                case messaging::REQ:
                    return RESPOND;

                default:
                    return DROP;
            }
        }

    } // messaging
} // sanla