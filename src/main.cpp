#include <Arduino.h>
#include "lora_sender.hpp"

String outgoing;              // outgoing message
byte msgCount = 0;            // count of outgoing messages
byte localAddress = 0xBB;     // address of this device
byte destination = 0xFF;      // destination to send to
long lastSendTime = 0;        // last send time
int interval = 2000;          // interval between sends

byte flags = 0x01;   // BRO
long packageId = 2147483647;

void setup() {
    Serial.begin(BAUDRATE);
    while (!Serial);
    Serial.println("Booting SanLa classic..");

    SPI.begin(SCK,MISO,MOSI,SS);
    LoRa.setPins(SS, RST, DI0);
    if (!LoRa.begin(BAND)) {
        Serial.println("SanLa communications init failed.");
        while (1);
    }

    LoRa.onReceive(onReceive);
    LoRa.receive();
    Serial.println("SanLa communications init succeeded.");
}

void loop() {
    if (millis() - lastSendTime > interval) {
        String message = "Foo walks into a bar baz qux moo";
        sendMessage(message);
        LoRa.receive();
        Serial.println("Sending: " + message);
        lastSendTime = millis();
        interval = random(4000) + 1000;
    }
}

void sendMessage(String outgoing) {
    LoRa.beginPacket();
    LoRa.write(flags);
    LoRa.write(packageId);
    LoRa.write(outgoing.length());
    LoRa.print(outgoing); // Note: Payload needs to be .print if String.
    LoRa.endPacket();
    msgCount++;
}

void onReceive(int packetSize) {
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