#ifndef SANLACLASSIC_UI_USERINTERFACE_H_
#define SANLACLASSIC_UI_USERINTERFACE_H_

#include <Arduino.h>
#include "common/typedefs.hpp"
#include "common/SanlaMessage.hpp"
#include "common/SanlaProcessor.hpp"

namespace sanla {
    namespace ui {

        using MessageHeader = sanla::messaging::MessageHeader;
        using SanlaMessage = sanla::messaging::SanlaMessagePackage;
        using Payload_t = sanla::messaging::sanlamessage::Payload_t;

        class UserInterface {
        public:
            UserInterface(){};

            /**
             * @brief Method for constructing a full package based on user input and sending it to MessageStore ready for broadcasting.
             * 
             * @param message User typed input message.
             */
            void sendUserMessage(std::string message);

            /**
             * @brief Display incoming message on screen.
             * 
             * @param message Incoming message.
             */
            static void displayMessage(String message);

            void registerProcessor(SanlaProcessor* processor);

        private:
            /**
             * @brief Build a message header based on user input.
             * 
             * @param _recipient_id Recipient group id.
             * @return sanla::MessageHeader Complete message header.
             */
            MessageHeader buildUserInputHeader(RecipientId_t);

            SanlaProcessor* sanla_processor_ptr = NULL;
        };

    } // ui
} // sanla


#endif