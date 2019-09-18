#include <string.h>
#include <sstream>
#include "common/SanlaMessage.hpp"
#include "common/SanlaPacket.hpp"
#include "HardwareSerial.h"

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

        void SanlaPacket::copy_headers_from_message(MessageHeader message_header, PacketPayload_t body) {
            header.message_id = message_header.message_id;
            header.sender_id = message_header.sender_id;
            header.recipient_id = message_header.recipient_id;
            header.payload_length = strlen(body); // Possible segfault due to non Null terminated string
        }
    } // messaging
} // sanla