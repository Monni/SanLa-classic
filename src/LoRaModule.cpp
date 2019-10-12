#include "LoRaModule.hpp"

namespace sanla {
namespace lora {

// Namespace global pointer to specific LoRaModule object
LoRaModule* ptrToLoraModule = NULL;

LoRaModule::LoRaModule() : _onReceive(NULL) {}

void LoRaModule::begin() {
    LoRa.setPins(SS, RST, DI0);
    if (!LoRa.begin(BAND)) {
        Serial.println("LoRaModule::begin -- Failed to start!");
        while (1);
    }
    setRadioParameters();

    ptrToLoraModule = this;
    LoRa.onReceive(onReceive);
    LoRa.receive();
}

void LoRaModule::setRadioParameters() {
    LoRa.setTxPower(TX_POWER);
    LoRa.setSpreadingFactor(S_FACTOR);
    LoRa.setSignalBandwidth(SIG_BNDWDTH);
    LoRa.setCodingRate4(CODING_RATE);
    LoRa.setPreambleLength(PREAMBL_LEN);
    LoRa.setSyncWord(SYNC_WORD);
}

bool LoRaModule::sendPacket(SanlaPacket &packet) {
    sanla::messaging::sanlapacket::SerializedPacket_t buffer{};
    sanla::messaging::htonSanlaPacket(packet, buffer);

    if (LoRa.beginPacket() == 1) {
        Serial.print("Sending packet for message ");
        Serial.print(packet.header.message_id);
        Serial.print(", sequence ");
        Serial.println(packet.header.payload_seq);

        for (auto const& packet_byte : buffer) {
            LoRa.write(packet_byte);
        }

        LoRa.endPacket(false);
        usleep(1000);

        // Revert back to listening mode.
        LoRa.receive();

        return true;
    }

    // Even upon failure ensure we are back at listening mode.
    LoRa.receive();

    return false;
}

void LoRaModule::onMessage(void(*callback)(SanlaMessage&)) {
    _onReceive = callback;
}

void LoRaModule::onPacket(int packetSize) {
    // FIXME. DO NOT REMOVE BELOW PRINTLN! WILL LEAD TO CORE PANIC!
    Serial.println("LoRaModule::onPacket");

    // Read incoming packet into a single byte array.
    char serializedPacket[packetSize];
    for (int i = 0; i < packetSize; i++) {
        serializedPacket[i] = (char)LoRa.read();
    }

    // Try to deserialize the byte array into a SanlaPacket.
    // TODO this probably needs some error handling.
    SanlaPacket packet = messaging::ntohSanlaPacket(serializedPacket);

    Serial.print("Received packet for message ");
    Serial.print(packet.header.message_id);
    Serial.print(", sequence ");
    Serial.println(packet.header.payload_seq);

    sanla_processor_ptr->ProcessPacket(packet);
    Serial.println("LoRaModule::onPacket -- Processing incoming packet done!");
}

void LoRaModule::onReceive(int packetSize) {
    if (packetSize == 0) return;

    if (ptrToLoraModule)
        ptrToLoraModule->onPacket(packetSize);
    else
        Serial.println("ERROR -- ptrToLoraModule is not set! FIXME!");
}

void LoRaModule::registerProcessor(SanlaProcessor* processor){
    sanla_processor_ptr = processor;
}

} // lora
} // sanla