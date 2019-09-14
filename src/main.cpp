#include "LoRaModule.hpp"
#include "common.hpp"
#include "common/SanlaProcessor.hpp"
#include "hw/LoraMsgIntepreter.hpp"
#include "ui/UserInterface.hpp"

long lastSendTime = 0;        // last send time
int interval = 2000;          // interval between sends
sanla::lora::LoRaModule lora;
sanla::ui::UserInterface user_interface;
sanla::hw_interfaces::LoraMessageIntepreter interpreter;

namespace sanla {
    auto g_sanlaProcessor = SanlaProcessorSingleton::Instance();
} // sanla


void setup() {
    // Set serial data transmission
    Serial.begin(sanla::common::BAUDRATE);
    while (!Serial);
    Serial.println("Starting SanLa Classic.");

    // Set Serial Peripheral Interface for controlling LoRa module
    SPI.begin(SCK,MISO,MOSI,SS);

    // Initialize customized LoRa module and register UI callback.
    lora.begin();
    lora.onMessage(user_interface.displayMessage);

    // Register SanLaProcessor into UI for callbacks.
    user_interface.registerProcessor(sanla::g_sanlaProcessor);

    Serial.println("SanLa Classic ready.");
}

void loop() {

    // Todo this is probably where the threading magic happens between UI and backend?

    // For testings purposes:
    if (millis() - lastSendTime > interval) {
        std::string message = "Foo walks into a bar baz qux moo, then another kazoo comes out of doo";

        // TODO for testing purposes, call UI's send method here on loop.
        user_interface.sendUserMessage(message);

        //lora.sendMessage(message); // TODO this should probably not be in lora but the module handling buffers.
        lastSendTime = millis();
        interval = random(4000) + 1000;
    }
}

