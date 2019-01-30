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
    
    // TODO Debugging prints for serialization
    Serial.println("");
    Serial.println(buffer[0], BIN);
    for (int x=1; x<5; x++) {
        Serial.print(buffer[x], BIN);
    };
    Serial.println("");

    for (int x=5; x<7; x++) {
        Serial.print(buffer[x], BIN);
    };
    Serial.println("");

    for (int x=7; x<15; x++) {
        Serial.print(buffer[x], BIN);
    };
    Serial.println("");

    // payload length
    for (int x=15; x<17; x++) {
        Serial.print(buffer[x], BIN);
    };
    Serial.println("");

    // payload seq
    for (int x=17; x<19; x++) {
        Serial.print(buffer[x], BIN);
    };
    Serial.println("");

    // payload chks
    for (int x=19; x<23; x++) {
        Serial.print(buffer[x], BIN);
    };
    Serial.println("");

    sanla::sanlamessage::SanlaPacketHeader ntohtest;
    ntohtest = sanla::sanlamessage::ntohSanlaPacketHeader(buffer);
    Serial.println("Ntoh: ");
    Serial.println(ntohtest.package_id);


    sanla::SanlaMessagePackage &&package = userInputPackage(message);

    // TODO package should be sent to some handler which figures out what to do with it once it's done.
    sanla::MessageHeader header = package.GetPackageHeader();
    std::stringstream serializedHeader;


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
    String incoming = "";
    while (LoRa.available()) {              // can't use readString() in callback, so
        incoming += (char)LoRa.read();      // add bytes one by one
    }
    Serial.println("Incoming: " + incoming);
    //Serial.print("Flags: 0x"); Serial.println(flags, HEX);
    //Serial.println("Seq: " + payload_seq);

    // Note: We should start writing unit tests for LoRaModule!
    if (ptrToLoraModule)
        ptrToLoraModule->_onReceive(incoming);
    else
        Serial.println("ptrToLoraModule is not set!");
    //if (inc_payload_length != incoming.length()) {
    //    Serial.println("error: message length does not match length");
    //    return;
    //}
}

} // lora
} // sanla