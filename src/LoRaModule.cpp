#include "LoRaModule.hpp"

namespace sanla {
namespace lora {

// Namespace global pointer to specific LoRaModule object
LoRaModule* ptrToLoraModule = NULL;

LoRaModule::LoRaModule() : _onReceive(NULL) {}

/**
 * @brief Initial boot function of a LoRa-module.
 * Sets the module into corresponding pins in ESP32 and handles the bootup routine.
 * Leaves the module into a state ready to receive messages.
 * 
 */
void LoRaModule::begin() {
    LoRa.setPins(SS, RST, DI0);
    if (!LoRa.begin(BAND)) {
        Serial.println("LoRaModule init failed.");
        while (1);
    }
    setRadioParameters();

    ptrToLoraModule = this;
    LoRa.onReceive(onMessage);
    LoRa.receive();
    Serial.println("LoRaModule init succeeded.");
}

/**
 * @brief Setup LoRa-radio with custom parameters.
 * The function is called during bootup routine.
 * 
 */
void LoRaModule::setRadioParameters() {
    LoRa.setTxPower(TX_POWER);
    LoRa.setSpreadingFactor(S_FACTOR);
    LoRa.setSignalBandwidth(SIG_BNDWDTH);
    LoRa.setCodingRate4(CODING_RATE);
    LoRa.setPreambleLength(PREAMBL_LEN);
    LoRa.setSyncWord(SYNC_WORD);
}

/**
 * @brief Build a message header based on user input.
 * 
 * @param _recipient_id Recipient group id.
 * @return sanla::MessageHeader Complete message header.
 */
sanla::MessageHeader buildUserInputHeader(RecipientId_t _recipient_id) {
    sanla::MessageHeader header;
    header.message_id = 1; // TODO generate. UUID?
    header.sender_id = 65535; // TODO generate. MAC?
    header.payload_chks = 1; // TODO calculate. Given as input?

    return header;
}

/**
 * @brief Build a message body based on user input.
 * 
 * @param _payload User input text.
 * @return sanla::MessageBody Complete message body.
 */
sanla::MessageBody buildUserInputBody(String _payload) {
    sanla::MessageBody body;
    strncpy(body.sender, "foo", sizeof("foo")); // TODO how to get this?
    strncpy(body.payload, _payload.c_str(), sizeof(_payload));

    return body;
}

/**
 * @brief Method for constructing a full package based on user input and sending it to MessageStore ready for broadcasting.
 * 
 * @param message User typed input message.
 */
void LoRaModule::sendMessage(String _user_input) {

    sanla::MessageHeader header = sanla::lora::buildUserInputHeader("sanla__"); // TODO where to get recipient id?
    sanla::MessageBody body = sanla::lora::buildUserInputBody(_user_input);
    sanla::SanlaMessagePackage package(header, body);

    // TODO send package to MessageStore for broadcasting.

    // TODO may be removed from here
    sanla:sanlamessage::SanlaPacket packet;
    packet.header.flags = sanla::sanlamessage::PRO;
    packet.header.message_id = 4294967295;
    packet.header.sender_id = 65535;
    strncpy(packet.header.recipient_id, "asdfasdf", sanla::sanlamessage::RECIPIENT_ID_MAX_SIZE);
    packet.header.message_payload_length = 65535;
    packet.header.payload_seq = 65535;
    packet.header.payload_chks = 4294967295;
    strncpy(packet.body.payload, "12345678901234567890", sizeof("12345678901234567890"));

    // TODO may be removed from here.
    char buffer[41]{};
    //sanla::sanlamessage::htonSanlaPacket(packet.header, packet.body, buffer);

    // TODO below send and revert to listening mode should be moved to a function inside handler. Is this handler?
    // Send.
    LoRa.beginPacket();
    LoRa.write((uint8_t*)buffer, 41);
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