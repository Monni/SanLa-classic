#include "message/MessageParser.hpp"

namespace sanla {
    namespace messaging {

        ActionsE DefineActionToPacket(const SanlaPacket &packet){
            switch(packet.header.flags){

                case messaging::BRO:
                    return RESPOND;
                
                case messaging::ACK:
                    return STORE;
                
                case messaging::PACPRO:
                    return RESPOND;
                
                case messaging::PACRES:
                    return RESPOND;

                case messaging::PACSEN:
                    return STORE;
                
                case messaging::PACREQ:
                    return RESPOND;

                case messaging::PROACK:
                    return RESPOND;

                default:
                    return DROP;
            }
        }

    } // messaging
} // sanla