#include "message/MessageParser.hpp"

namespace sanla {
    namespace sanlamessage {

        void MessageParser::setDropAction(FuncPtr func){
            actions.drop = func;
        }
        void MessageParser::setRespondAction(FuncPtr func){
            actions.respond = func;
        }
        void MessageParser::setStoreAction(FuncPtr func){
            actions.store = func;
        }
        FuncPtr MessageParser::ParseMessage(SanlaPacket packet){
            switch(packet.flags){

                case sanlamessage::BRO:
                    return actions.respond;
                
                case sanlamessage::ACK:
                    return actions.store;
                
                case sanlamessage::PACPRO:
                    return actions.respond;
                
                case sanlamessage::PACRES:
                    return actions.respond;

                case sanlamessage::PACSEN:
                    return actions.store;
                
                case sanlamessage::PACREQ:
                    return actions.respond;

                case sanlamessage::PROACK:
                    return actions.respond;

                default:
                    return actions.drop;
            }
        }

    } // sanlamessage
} // sanla