#include "LoRaModule.hpp"

using namespace sanla::lora;

LoRaModule::LoRaModule() : _onReceive(NULL) {}

// Startup the module
void LoRaModule::begin() {
    setRadioParameters();
    if (!LoRa.begin(BAND)) {
        Serial.println("LoRaModule init failed.");
        while (1);
    }

    LoRa.onReceive(onMessage);
    //LoRa.receive();
    Serial.println("LoRaModule init succeeded.");
}

void LoRaModule::setRadioParameters() {
    // Set custom radio parameters.
    LoRa.setPins(SS, RST, DI0);
    LoRa.setTxPower(TX_POWER);
    LoRa.setSpreadingFactor(S_FACTOR);
    LoRa.setSignalBandwidth(SIG_BNDWDTH);
    LoRa.setCodingRate4(CODING_RATE);
    LoRa.setPreambleLength(PREAMBL_LEN);
    LoRa.setSyncWord(SYNC_WORD);
}

void LoRaModule::sendMessage(String message) {
    // TODO Temporary solution. Construct packet header.
    byte flags = 0x01;   // BRO
    long packageId = 2147483647;

    Serial.println("Sending: " + message);

    LoRa.beginPacket();
    LoRa.write(flags);
    LoRa.write(packageId);
    LoRa.write(message.length());
    LoRa.print(message); // Note: Payload needs to be .print if String.
    LoRa.endPacket();

    // Revert back to listening mode
    LoRa.receive();
    LoRaModule::packageReceived(message);
}

void LoRaModule::onPackage(void(*callback)(String)) {
    LoRaModule::_onReceive = callback;
}

void LoRaModule::packageReceived(String message) {
    // TODO Validate if I'm recipient
    if (LoRaModule::_onReceive) {
        LoRaModule::_onReceive(message);
    }

}

void LoRaModule::packetHeader() {
    // TODO construct packet header here or somewhere in SanlaMessage?
}

void LoRaModule::onMessage(int packetSize) {
    if (packetSize == 0) return;

    // Read packet header bytes:
    byte inc_flags = LoRa.read();
    long inc_packageId = LoRa.read();
    int inc_payload_length = LoRa.read();

    // Read packet payload
    String incoming = "";
    while (LoRa.available()) {              // can't use readString() in callback, so
        incoming += (char)LoRa.read();      // add bytes one by one
    }

    // Some initial validation thingy. Checksum later
    if (inc_payload_length != incoming.length()) {
        Serial.println("error: message length does not match length");
        return;
    }

    // if message is for this device, or broadcast, print details:
    Serial.println("Received message:");
    Serial.println("Message ID: " + String(inc_packageId));
    Serial.print("Message flags: ");
    Serial.println(inc_flags, HEX);
    Serial.println("Message length: " + String(inc_payload_length));
    Serial.println("Message: " + incoming);
    Serial.println("RSSI: " + String(LoRa.packetRssi()));
    Serial.println("Snr: " + String(LoRa.packetSnr()));
    Serial.println();
}
