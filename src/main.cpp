#include "LoRaModule.hpp"
#include "common.hpp"
#include "common/SanlaProcessor.hpp"
#include "hw/LoraPacketIntepreter.hpp"
#include "ui/UserInterface.hpp"

long lastSendTime = 0;        // last send time
int interval = 50000;          // interval between sends

long uplinkbuffer_lastSendTime = 0;
int uplinkbuffer_interval = 10000;

sanla::lora::LoRaModule lora;
sanla::ui::UserInterface user_interface;
sanla::hw_interfaces::LoraPacketIntepreter interpreter;

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

    //lora.onMessage(user_interface.displayMessage);

    // Register SanLaProcessor into components for callbacks.
    user_interface.registerProcessor(sanla::g_sanlaProcessor);
    lora.registerProcessor(sanla::g_sanlaProcessor);

    Serial.println("SanLa Classic ready.");
}

void loop() {

    // Todo this is probably where the threading magic happens between UI and backend?

    // For testings purposes:
    if (millis() - lastSendTime > interval) {
        std::string message = "Foo walks into a bar baz qux moo, then another corge comes out of garply. But what happens when the goo is on a doo?";

        // TODO for testing purposes, call UI's send method here on loop.
        user_interface.sendUserMessage(message);

        lastSendTime = millis();
    } else if (millis() - uplinkbuffer_lastSendTime > uplinkbuffer_interval) {
        
        // Try to clear uplinkbuffer at periods.
        sanla::g_sanlaProcessor->SendUplinkBuffer();

        uplinkbuffer_lastSendTime = millis();
    }
}

