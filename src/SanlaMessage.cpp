#include <string.h>
#include <sstream>
#include "common/SanlaMessage.hpp"

namespace sanla {

SanlaMessagePackage::SanlaMessagePackage(uint32_t _package_id, uint16_t _sender_id, uint32_t _payload_chks, const MessageBody _body ) {
    header.package_id = _package_id;
    header.sender_id = _sender_id;
    header.payload_chks = _payload_chks;
    //header.recipient_id = _recipient_id;
    body = _body;
}

SanlaMessagePackage::SanlaMessagePackage(MessageHeader _header, MessageBody _body) {
    header = _header;
    body = _body;
}

uint8_t SanlaMessagePackage::GetPackageLength() {
    // TODO calculate real payload length
    return 123;
}

MessageHeader& SanlaMessagePackage::GetPackageHeader() {
    return header;
}

MessageBody& SanlaMessagePackage::GetPackageBody() {
    return body;
}

namespace sanlamessage{
    void SanlaPacket::copy_headers_from_message(MessageHeader header, MessageBody body) {
        flags = header.flags;
        package_id = header.package_id;
        sender_id = header.sender_id;
        strcpy(recipient_id, header.recipient_id.c_str());
        package_payload_length = strlen(body.payload); // Possible segfault due to non Null terminated string
        payload_seq = header.payload_seq;
        payload_chks = header.payload_chks;

    }
} // sanlamessage
} // sanla