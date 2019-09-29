#ifndef SANLACLASSIC_UI_USERINTERFACE_H_
#define SANLACLASSIC_UI_USERINTERFACE_H_

#include <HardwareSerial.h>
#include <random>
#include <time.h>
#include "common/typedefs.hpp"
#include "common/SanlaMessage.hpp"
#include "common.hpp"

namespace sanla {
    namespace ui {

        using MessageHeader = sanla::messaging::MessageHeader;
        using SanlaMessage = sanla::messaging::SanlaMessage;
        using Payload_t = sanla::messaging::sanlamessage::Payload_t;

        class UserInterface {
        public:
            UserInterface(){};
            UserInterface(void *processor_ptr) : m_sanla_processor_ptr(processor_ptr) {};

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
            static void displayMessage(SanlaMessage&);

            /**
             * @brief Set the Recipient Id object.
             * 
             */
            void setGroupId(RecipientId_t);

            /**
             * @brief Get the Recipient Id object
             * 
             * @return RecipientId_t 
             */
            RecipientId_t getGroupId();

        private:
            RecipientId_t recipientId = 65535; // Default global channel.

            /**
             * @brief Build a message header based on user input.
             * 
             * @param _recipient_id Recipient group id.
             * @return sanla::MessageHeader Complete message header.
             */
            MessageHeader buildUserInputHeader(RecipientId_t);

            // This is later casted to SanlaProcessor
            void* m_sanla_processor_ptr = nullptr;
        };

    } // ui
} // sanla


#endif