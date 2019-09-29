#include <stdlib_noniso.h>
#include "LoRaModule.hpp"
#include "common.hpp"
#include "common/SanlaProcessor.hpp"
#include "ui/UserInterface.hpp"

// Try to send packets from uplinkbuffer each 5 seconds.
long uplinkbuffer_lastSendTime = 0;
int uplinkbuffer_interval = 5000;

std::string inputString = "";
bool stringComplete = false;

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
        std::string actionStr = inputString.substr(0, 1);
        int action = std::atoi(actionStr.c_str());
        std::string param = inputString.substr(2);

        switch (action)
        {
        case sanla::common::SEND_MESSAGE:
            user_interface->sendUserMessage(param);
            break;
        case sanla::common::SET_GROUP_ID:
            user_interface->setGroupId(std::atoi(param.c_str()));
            break;
        
        default:
            Serial.print("Unknown command received: ");
            Serial.println(inputString.c_str());
            break;
        }

        inputString = "";
        stringComplete = false;
    }

    if (millis() - uplinkbuffer_lastSendTime > uplinkbuffer_interval) {
        sanla::g_sanlaProcessor->SendUplinkBuffer();
        uplinkbuffer_lastSendTime = millis();
    }

}

