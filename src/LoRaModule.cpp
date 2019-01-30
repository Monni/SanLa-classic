#include "LoRaModule.hpp"

namespace sanla {
namespace lora {

// Namespace global pointer to specific LoRaModule object
LoRaModule* ptrToLoraModule = NULL;

LoRaModule::LoRaModule() : _onReceive(NULL) {}

void LoRaModule::begin() {
    // Startup the LoRa module
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

void LoRaModule::setRadioParameters() {
    // Set custom radio parameters.
    LoRa.setTxPower(TX_POWER);
    LoRa.setSpreadingFactor(S_FACTOR);
    LoRa.setSignalBandwidth(SIG_BNDWDTH);
    LoRa.setCodingRate4(CODING_RATE);
    LoRa.setPreambleLength(PREAMBL_LEN);
    LoRa.setSyncWord(SYNC_WORD);
}

/**
 * @brief Construct SanlaMessage package based on user input
 * 
 * @param message User typed message.
 * @return sanla::SanlaMessagePackage Constructed package. 
 */
sanla::SanlaMessagePackage LoRaModule::userInputPackage(String message) {
    // Create a message package based on user input.

    // Create header
    sanla::MessageHeader header;
    header.sender_id = 1; // TODO figure out method for sender_id
    header.payload_chks = 1; // TODO method for calculating payload_chks
    header.package_id = 1; // TODO generate package_id
    header.recipient_id = "Foo"; // TODO this needs to come from somewhere

    // Create body
    sanla::MessageBody body = {
            "Foo",   // TODO instead of hard-coding, username should be used.
            message.c_str()
    };

    // For some cucking reason return this.
    return {header, body};
}

/**
 * @brief Method for constructing a full package based on user input, ready for broadcasting.
 * 
 * @param message User typed input message.
 */
void LoRaModule::sendMessage(String message) {

    // TODO remove below test packet.
    sanla:sanlamessage::SanlaPacket packet;
    packet.header.flags = sanla::sanlamessage::PRO;
    packet.header.package_id = 4294967295;
    packet.header.sender_id = 65535;
    strncpy(packet.header.recipient_id, "asdfasdf", sanla::sanlamessage::RECIPIENT_ID_MAX_SIZE);
    packet.header.package_payload_length = 65535;
    packet.header.payload_seq = 65535;
    packet.header.payload_chks = 4294967295;
    strncpy(packet.body.payload, "12345678901234567890", sizeof("12345678901234567890"));

    char buffer[23]{};
    sanla::sanlamessage::htonSanlaPacket(packet.header, packet.body, buffer);
    
    sanla::SanlaMessagePackage &&package = userInputPackage(message);

    // Send    
    LoRa.beginPacket();
    LoRa.write((uint8_t*)buffer, 23);
    LoRa.endPacket();

    // Revert back to listening mode.
    LoRa.receive();
}

void LoRaModule::onPackage(void(*callback)(String)) {
    _onReceive = callback;
}

void LoRaModule::packageReceived(String message) {
    // TODO Validate if I'm recipient and if:
    if (_onReceive) {
        _onReceive(message);
    }

    // TODO Broadcast

}

void LoRaModule::onMessage(int packetSize) {
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