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
    LoRa.onReceive(onPacket);
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

bool LoRaModule::sendPacket(SanlaPacket packet) {
    
    sanla::messaging::sanlapacket::SerializedPacket_t buffer{};
    sanla::messaging::htonSanlaPacket(packet, buffer);
    
    // Send.
    if (LoRa.beginPacket()) {
        LoRa.write((uint8_t*)buffer, sanla::messaging::sanlapacket::PACKET_MAX_SIZE);
        LoRa.endPacket();

        // Revert back to listening mode.
        LoRa.receive();

        return true;
    }

    // Even upon failure ensure we are back at listening mode.
    LoRa.receive();

    return false;
}

void LoRaModule::onMessage(void(*callback)(String)) {
    _onReceive = callback;
}

void LoRaModule::packageReceived(String message) {
    // TODO Move to handler or interpreter and finish this method.
    if (_onReceive) {
        _onReceive(message);
    }
}

void LoRaModule::onPacket(int packetSize) {
    Serial.println("LoRaModule::onPacket");
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