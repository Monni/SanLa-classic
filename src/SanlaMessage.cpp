#include <sstream>
#include "common/SanlaMessage.hpp"

using namespace sanla;

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