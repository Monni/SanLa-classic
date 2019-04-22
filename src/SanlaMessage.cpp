#include <string.h>
#include <sstream>
#include "common/SanlaMessage.hpp"
#include "common/SanlaPacket.hpp"

namespace sanla {

SanlaMessagePackage::SanlaMessagePackage(PackageId_t _message_id, SenderId_t _sender_id, PayloadChecksum_t _payload_chks, const MessageBody _body ) {
    header.message_id = _message_id;
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
        PackageId_t package_id = header.message_id;
        SenderId_t sender_id = header.sender_id;
        PayloadChecksum_t payload_chks = header.payload_chks;
        char recipient_id[header.recipient_id.length()+1];
        strcpy(recipient_id, header.recipient_id.c_str());
        PayloadLength_t message_payload_length = strlen(body.payload); // Possible segfault due to non Null terminated string

    }
} // sanlamessage
} // sanla