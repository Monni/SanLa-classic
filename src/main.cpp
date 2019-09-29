#include "LoRaModule.hpp"
#include "common.hpp"
#include "common/SanlaProcessor.hpp"
#include "ui/UserInterface.hpp"

// Try to send packets from uplinkbuffer each 10 seconds.
long uplinkbuffer_lastSendTime = 0;
int uplinkbuffer_interval = 10000;

std::string inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

sanla::lora::LoRaModule lora;
sanla::ui::UserInterface *user_interface = nullptr;

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

    // Register SanLaProcessor into components for callbacks.
    lora.registerProcessor(sanla::g_sanlaProcessor);
    user_interface = sanla::g_sanlaProcessor->getInterfacePtr();

    Serial.println("SanLa Classic ready.");
}

void serialEvent() {
    while (Serial.available()) {
        char inChar = (char)Serial.read();
        inputString += inChar;
        if (inChar == '\n') {
            stringComplete = true;
        }
    }
}

void loop() {

    serialEvent();
    if (stringComplete) {
        user_interface->sendUserMessage(inputString);

        inputString = "";
        stringComplete = false;
    }

    if (millis() - uplinkbuffer_lastSendTime > uplinkbuffer_interval) {
        
        // Periodically try to clear UplinkBuffer.
        sanla::g_sanlaProcessor->SendUplinkBuffer();

        uplinkbuffer_lastSendTime = millis();
    }

}

