#include "ui/UserInterface.hpp"

namespace sanla {
    namespace ui {

        MessageHeader UserInterface::buildUserInputHeader(RecipientId_t _recipient_id) {
            MessageHeader header;
            header.message_id = 1; // TODO generate. UUID?
            header.sender_id = 65535; // TODO generate. MAC?
            header.recipient_id = _recipient_id;

            return header;
        };

        Payload_t* UserInterface::buildUserInputBody(String _payload) {
            sanla::messaging::sanlamessage::Payload_t* body{};
            strcpy(*body, _payload.c_str());

            return body;
        };

        void UserInterface::sendUserMessage(String _input) {
            
            // TODO get recipient id and use instead of 65535
            SanlaMessage message(buildUserInputHeader(65535), *buildUserInputBody(_input));

            sanla_processor_ptr->HandleMessage(message);
        };

        void UserInterface::displayMessage(String message) {
            Serial.println("*** Incoming message into UI ***");
            Serial.println(message);
        };

        void UserInterface::registerProcessor(SanlaProcessor* processor){
            sanla_processor_ptr = processor;
        }

    } // ui
} // sanla