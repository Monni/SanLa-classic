#include "message/MessageParser.hpp"

namespace sanla {
    namespace sanlamessage {

        uint32_t MessageParser::ParseMessage(SanlaPacket packet){
            switch(packet.header.flags){

                case sanlamessage::BRO:
                    return RESPOND;
                
                case sanlamessage::ACK:
                    return STORE;
                
                case sanlamessage::PACPRO:
                    return RESPOND;
                
                case sanlamessage::PACRES:
                    return RESPOND;

                case sanlamessage::PACSEN:
                    return STORE;
                
                case sanlamessage::PACREQ:
                    return RESPOND;

                case sanlamessage::PROACK:
                    return RESPOND;

                default:
                    return DROP;
            }
        }

    } // sanlamessage
} // sanla