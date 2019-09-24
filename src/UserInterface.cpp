#include "ui/UserInterface.hpp"
#include "common/SanlaProcessor.hpp" // This can't be moved into the header.

namespace sanla {
    namespace ui {

        MessageHeader UserInterface::buildUserInputHeader(RecipientId_t _recipient_id) {
            MessageHeader header;

            // Generate random message id.
            std::mt19937 rng;
            rng.seed(time(0));
            std::uniform_int_distribution<MessageId_t> uint16_dist65535(0, 65535);
            header.message_id = uint16_dist65535(rng);

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
            if (m_sanla_processor_ptr != nullptr) {
                auto processor = static_cast<SanlaProcessor*>(m_sanla_processor_ptr);
                processor->HandleMessage(message);
            }
            else {
                // Throw some error here
            }
        };

        void UserInterface::displayMessage(SanlaMessage& message) {
            Serial.println("");
            Serial.println("*** Incoming message into UI ***");
            Serial.print("Message ID: ");
            Serial.println(message.GetMessageHeader().message_id);
            Serial.print("Sender ID: ");
            Serial.println(message.GetMessageHeader().sender_id);
            Serial.print("Recipient ID: ");
            Serial.println(message.GetMessageHeader().recipient_id);
            Serial.print("Body: ");
            Serial.println(message.GetMessageBody());
            Serial.println("");
        };

    } // ui
} // sanla