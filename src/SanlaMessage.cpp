#include <sstream>
#include "common/SanlaMessage.hpp"

using namespace sanla;

SanlaMessagePackage::SanlaMessagePackage(u_char _flags, uint8_t _payload_seq, 
uint16_t _length, uint64_t _sender_id, uint64_t _payload_chks, uint32_t _package_id,
std::string _recipient_id, const MessageBody _body ) {
    header.flags = _flags;
    header.payload_seq = _payload_seq;
    header.length = _length;
    header.sender_id = _sender_id;
    header.payload_chks = _payload_chks;
    header.package_id = _package_id;
    header.recipient_id = _recipient_id;
    body = _body;

}

SanlaMessagePackage::SanlaMessagePackage(MessageHeader _header, MessageBody _body) {
    header = _header;
    body = _body;
}

uint8_t SanlaMessagePackage::GetPackageLength() {
    return header.payload_seq;
}

uint16_t SanlaMessagePackage::GetTotalPackageLength() {
    return header.length;
}

MessageHeader& SanlaMessagePackage::GetPackageHeader() {
    return header;
}

MessageBody& SanlaMessagePackage::GetPackageBody() {
    return body;
}