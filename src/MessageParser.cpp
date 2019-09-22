#include "message/MessageParser.hpp"

namespace sanla {
    namespace messaging {

        ActionsE DefineActionToPacket(const SanlaPacket &packet){
            /*
            TODO
                - Validoi paketti on meidan. Miten??
                - Validoi saapuneen paketin payload length ja checksum. Jos ei tasmaa, return REQUEST.
            */

            switch(packet.header.flags){

                case messaging::REQ:
                    return RESPOND;

                default:
                    return RECEIVE;
            }
        }

    } // messaging
} // sanla