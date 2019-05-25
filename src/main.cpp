#include "LoRaModule.hpp"
#include "common.hpp"
#include "common/SanlaProcessor.hpp"
#include "hw/LoraMsgIntepreter.hpp"

long lastSendTime = 0;        // last send time
int interval = 2000;          // interval between sends
sanla::lora::LoRaModule lora;
sanla::hw_interfaces::LoraMessageIntepreter interpreter;

namespace sanla {
    auto g_sanlaProcessor = SanlaProcessorSingleton::Instance();

namespace common {
    void displayMessage(String message) {
        Serial.println("Callback: " + message);
        }
} // common
} // sanla


void setup() {
    // Set serial data transmission
    Serial.begin(sanla::common::BAUDRATE);
    while (!Serial);
    Serial.println("Starting SanLa Classic.");

    // Set Serial Peripheral Interface for controlling LoRa module
    SPI.begin(SCK,MISO,MOSI,SS);

    // Initialize customized LoRa module
    lora.begin();
    lora.onPackage(sanla::common::displayMessage);
    Serial.println("SanLa Classic ready.");
}

void loop() {

    // Todo this is probably where the threading magic happens between UI and backend?

    // For testings purposes:
    if (millis() - lastSendTime > interval) {
        String message = "Foo walks into a bar baz qux moo";
        lora.sendMessage(message); // TODO this should probably not be in lora but the module handling buffers.
        lastSendTime = millis();
        interval = random(4000) + 1000;
    }
}

