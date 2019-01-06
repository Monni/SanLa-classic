#include "LoRaModule.hpp"

using namespace sanla::lora;

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

sanla::SanlaMessagePackage LoRaModule::userInputPackage(String message) {
    // Create a message package based on user input.

    // Create header
    // TODO figure out these values
    sanla::MessageHeader header;
    header.flags = 0x1;
    header.payload_seq = 1;
    header.length = 1;
    header.sender_id = 1;
    header.payload_chks = 1;
    header.package_id = 1;
    header.recipient_id = "foo";

    // Create body
    sanla::MessageBody body = {
            "Foo",   // TODO instead of hard-coding, username should be used.
            message.c_str()
    };

    // For some cucking reason return this.
    return {header, body};
}

void LoRaModule::sendMessage(String message) {
    // This method is only for generating a broadcast message based on user input.
    // No other functions should use this one. This should call the outward stream for packages.

    sanla::SanlaMessagePackage &&package = userInputPackage(message);

    // TODO package should be sent to some handler which figures out what to do with it once it's done.
    sanla::MessageHeader header = package.GetPackageHeader();
    std::stringstream serializedHeader;
    header.serialize(serializedHeader);

    Serial.println("Sending message: " + message);

    LoRa.beginPacket();
    LoRa.write((uint8_t*)serializedHeader.str().c_str(), sizeof(serializedHeader.str().c_str())); // TODO WHAT? Now this goes as characters and in receiving end ie. uint16_t translates as uint8_t if suitable value.
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
