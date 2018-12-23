#include "LoRaModule.hpp"
#include <thread>

using namespace sanla::lora;

LoRaModule::LoRaModule() : _onReceive(NULL) {}

// Startup the module
void LoRaModule::begin() {
    LoRa.setPins(SS, RST, DI0);
    if (!LoRa.begin(BAND)) {
        Serial.println("LoRaModule init failed.");
        while (1);
    }
    setRadioParameters();

    LoRa.onReceive(onMessage);
    //LoRa.receive();
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

void LoRaModule::sendMessage(String message) {
    // TODO Temporary solution.
    sanla::MessageHeader header = {
            0x1,1,1,1,1,1,"asd"
    };
    sanla::MessageBody body = {"Foo", message.c_str()};
    sanla::SanlaMessagePackage package(header, body);

    Serial.println("Sending: " + message);

    LoRa.beginPacket();
    LoRa.write((uint8_t*)&package.GetPackageHeader(), sizeof(package.GetPackageHeader()));
    LoRa.endPacket();

    // Revert back to listening mode
    LoRa.receive();
    //LoRaModule::packageReceived(message); // TODO works here but not in onMessage
}

void LoRaModule::onPackage(void(*callback)(String)) {
    LoRaModule::_onReceive = callback;
}

void LoRaModule::packageReceived(String message) {
    // TODO Validate if I'm recipient and if:
    if (LoRaModule::_onReceive) {
        LoRaModule::_onReceive(message);
    }

    // TODO Broadcast

}

void LoRaModule::onMessage(int packetSize) {
    if (packetSize == 0) return;

    // Read packet payload
    String incoming = "";
    while (LoRa.available()) {              // can't use readString() in callback, so
        incoming += (char)LoRa.read();      // add bytes one by one
    }
    Serial.println("Incoming: " + incoming);
    LoRaModule::_onReceive(incoming);

    // Some initial validation thingy. Checksum later
    //if (inc_payload_length != incoming.length()) {
    //    Serial.println("error: message length does not match length");
    //    return;
    //}
}
