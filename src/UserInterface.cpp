#include "ui/UserInterface.hpp"

namespace sanla {
    namespace ui {

        MessageHeader UserInterface::buildUserInputHeader(RecipientId_t _recipient_id) {
            MessageHeader header;

            // Generate random message id.
            std::mt19937 rng;
            rng.seed(time(0));
            std::uniform_int_distribution<MessageId_t> uint16_dist65535(0, 65535);
            header.message_id = uint16_dist65535(rng);

            header.sender_id = 65535; // TODO generate. MAC?
            header.recipient_id = _recipient_id;

            return header;
        };

        void UserInterface::sendUserMessage(std::string _input) {
            Serial.println("UserInterface::sendUserMessage");

            // Build body from input for SanlaMessage.
            Payload_t payload_arr{};
            strcpy(payload_arr, _input.c_str());
            
            // TODO get recipient id and use instead of 65535.
            SanlaMessage message(buildUserInputHeader(65535), payload_arr);

            // Send to SanlaProcessor.
            sanla_processor_ptr->HandleMessage(message);
        };

        void UserInterface::displayMessage(std::string message) {
            Serial.println("*** Incoming message into UI ***");
            Serial.println(message.c_str());
        };

        void UserInterface::registerProcessor(SanlaProcessor* processor){
            sanla_processor_ptr = processor;
        }

    } // ui
} // sanla