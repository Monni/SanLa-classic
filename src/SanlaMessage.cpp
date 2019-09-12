#include <string.h>
#include <sstream>
#include "common/SanlaMessage.hpp"
#include "common/SanlaPacket.hpp"

namespace sanla {
    namespace messaging {

        SanlaMessagePackage::SanlaMessagePackage(MessageId_t _message_id, SenderId_t _sender_id, RecipientId_t _recipient_id, Payload_t _body ) {
            header.message_id = _message_id;
            header.sender_id = _sender_id;
            header.recipient_id = _recipient_id;
            strcpy(body, _body);
        }

        SanlaMessagePackage::SanlaMessagePackage(MessageHeader _header, Payload_t _body) {
            header = _header;
            strcpy(body, _body);
        }

        uint16_t SanlaMessagePackage::GetPackageLength() {
            return strlen(body);
        }

        MessageHeader& SanlaMessagePackage::GetPackageHeader() {
            return header;
        }

        Payload_t& SanlaMessagePackage::GetPackageBody() {
            return body;
        }

        void SanlaPacket::copy_headers_from_message(MessageHeader header, Payload_t body) {
                MessageId_t message_id = header.message_id;
                SenderId_t sender_id = header.sender_id;
                RecipientId_t recipient_id = header.recipient_id;
                PayloadLength_t payload_length = strlen(body); // Possible segfault due to non Null terminated string
        }
    } // messaging
} // sanla