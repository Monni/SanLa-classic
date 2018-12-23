#include "LoRaModule.hpp"
#include "common.hpp"

long lastSendTime = 0;        // last send time
int interval = 2000;          // interval between sends
sanla::lora::LoRaModule lora;


void displayMessage(String message) {
    Serial.println("Callback: " + message);
}

void setup() {
    Serial.begin(BAUDRATE);
    while (!Serial);
    Serial.println("Booting SanLa classic..");

    SPI.begin(SCK,MISO,MOSI,SS);
    lora.begin();
    lora.onPackage(displayMessage);

}

void loop() {
    if (millis() - lastSendTime > interval) {
        String message = "Foo walks into a bar baz qux moo";
        lora.sendMessage(message);
        lastSendTime = millis();
        interval = random(4000) + 1000;
    }
}

