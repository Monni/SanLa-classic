#include "LoRaModule.hpp"

namespace sanla {
namespace lora {

// Namespace global pointer to specific LoRaModule object
LoRaModule* ptrToLoraModule = NULL;

LoRaModule::LoRaModule() : _onReceive(NULL) {}

void LoRaModule::begin() {
    Serial.println("Starting LoRa module.");
    LoRa.setPins(SS, RST, DI0);
    if (!LoRa.begin(BAND)) {
        Serial.println("Failed to start LoRa module!");
        while (1);
    }
    setRadioParameters();

    ptrToLoraModule = this;
    LoRa.onReceive(onMessage);
    LoRa.receive();
    Serial.println("LoRa module started.");
}

void LoRaModule::setRadioParameters() {
    LoRa.setTxPower(TX_POWER);
    LoRa.setSpreadingFactor(S_FACTOR);
    LoRa.setSignalBandwidth(SIG_BNDWDTH);
    LoRa.setCodingRate4(CODING_RATE);
    LoRa.setPreambleLength(PREAMBL_LEN);
    LoRa.setSyncWord(SYNC_WORD);
}

sanla::MessageHeader buildUserInputHeader(RecipientId_t _recipient_id) {
    sanla::MessageHeader header;
    header.message_id = 1; // TODO generate. UUID?
    header.sender_id = 65535; // TODO generate. MAC?
    header.payload_chks = 1; // TODO calculate. Given as input?

    return header;
}

sanla::MessageBody buildUserInputBody(String _payload) {
    sanla::MessageBody body;
    strncpy(body.sender, "foo", sizeof("foo")); // TODO how to get this?
    strncpy(body.payload, _payload.c_str(), sizeof(_payload));

    return body;
}

void LoRaModule::sendMessage(String _input) {

    sanla::MessageHeader header = sanla::lora::buildUserInputHeader("sanla__"); // TODO where to get recipient id?
    sanla::MessageBody body = sanla::lora::buildUserInputBody(_input);
    sanla::SanlaMessagePackage package(header, body);

    // TODO send package to MessageStore for broadcasting.

    // TODO may be removed from here
    sanla:messaging::SanlaPacket packet;
    packet.header.flags = sanla::messaging::PRO;
    packet.header.message_id = 4294967295;
    packet.header.sender_id = 65535;
    strncpy(packet.header.recipient_id, "asdfasdf", sanla::messaging::RECIPIENT_ID_MAX_SIZE);
    packet.header.message_payload_length = 65535;
    packet.header.payload_seq = 65535;
    packet.header.payload_chks = 4294967295;
    strncpy(packet.body, "12345678901234567890", sizeof("12345678901234567890"));

    // TODO may be removed from here.
    sanla::messaging::sanlapacket::SerializedPacket_t buffer{};
    //sanla::messaging::htonSanlaPacket(packet.header, packet.body, buffer);

    // TODO below send and revert to listening mode should be moved to a function inside handler. Is this handler?
    // Send.
    LoRa.beginPacket();
    LoRa.write((uint8_t*)buffer, sanla::messaging::sanlapacket::PACKET_MAX_SIZE);
    LoRa.endPacket();

    // Revert back to listening mode.
    LoRa.receive();
}

void LoRaModule::onPackage(void(*callback)(String)) {
    _onReceive = callback;
}

void LoRaModule::packageReceived(String message) {
    // TODO Move to handler or interpreter and finish this method.
    if (_onReceive) {
        _onReceive(message);
    }
}

void LoRaModule::onMessage(int packetSize) {
    Serial.println("onMessage");
    if (packetSize == 0) return;

    byte byteArray[packetSize];
    for (int i = 0; i < packetSize; i++) {
        byteArray[i] = (char)LoRa.read();
    }

    // Read packet payload
    // TODO this needs to be forwarded into DownlinkBuffer.
    String incoming = "";
    while (LoRa.available()) {              // can't use readString() in callback, so
        incoming += (char)LoRa.read();      // add bytes one by one
    }
    Serial.println("Incoming: " + incoming);

    // Note: We should start writing unit tests for LoRaModule!
    if (ptrToLoraModule)
        ptrToLoraModule->_onReceive(incoming);
    else
        Serial.println("ptrToLoraModule is not set!");
}

} // lora
} // sanla