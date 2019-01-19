#include <string>
#include <Arduino.h>
#include <SPI.h>

#ifndef SANLACLASSIC_COMMON_H_
#define SANLACLASSIC_COMMON_H_

namespace sanla {
    namespace common {
    // General HW definitions
    const long BAUDRATE = 115200;
    void displayMessage(String message);
    };
};
#endif