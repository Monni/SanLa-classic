#include "ui/UserInterface.hpp"

namespace sanla {
    namespace ui {

        MessageHeader UserInterface::buildUserInputHeader(RecipientId_t recipient_id) {
            MessageHeader header;
                header.message_id = 1; // TODO generate. UUID?
                header.sender_id = 65535; // TODO generate. MAC?
                header.payload_chks = 1; // TODO to be removed from header.

            return header;
        };

        Payload_t* UserInterface::buildUserInputBody(String _payload) {
            sanla::messaging::sanlamessage::Payload_t* body;
            strcpy(*body, _payload.c_str());

            return body;
        };

        void UserInterface::sendUserMessage(String _input) {
            MessageHeader header = buildUserInputHeader("sanla__"); // TODO where to get recipient id?
            SanlaMessage message(header, *buildUserInputBody(_input));

            sanla_processor_ptr->HandleUplinkMessage(message);

        };

        void UserInterface::registerProcessor(SanlaProcessor* processor){
            sanla_processor_ptr = processor;
        }

    } // ui
} // sanla