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
    LoRa.onReceive(onReceive);
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

void LoRaModule::onMessage(void(*callback)(SanlaPacket)) {
    _onReceive = callback;
}

void LoRaModule::onPacket(int packetSize) {
    Serial.println("LoRaModule::onPacket");

    // Read incoming packet into a single byte array.
    char serializedPacket[packetSize];
    for (int i = 0; i < packetSize; i++) {
        serializedPacket[i] = (char)LoRa.read();
    }

    // Try to deserialize the byte array into a SanlaPacket.
    // TODO this probably needs some error handling.
    SanlaPacket packet = messaging::ntohSanlaPacket(serializedPacket);

    Serial.print("Packet arrived. Payload: ");
    Serial.println(packet.body);

    Serial.println("Sending packet to processor..");
    sanla_processor_ptr->ProcessPacket(packet);
    Serial.println("Processing done!");
}

void LoRaModule::onReceive(int packetSize) {
    if (packetSize == 0) return;

    if (ptrToLoraModule)
        ptrToLoraModule->onPacket(packetSize);
    else
        Serial.println("ptrToLoraModule is not set!");
}

void LoRaModule::registerProcessor(SanlaProcessor* processor){
    sanla_processor_ptr = processor;
}

} // lora
} // sanla