#ifndef SANLACLASSIC_COMMON_H_
#define SANLACLASSIC_COMMON_H_

#include <string>
#include <SPI.h>

namespace sanla {
    namespace common {
    // General HW definitions
    const long BAUDRATE = 115200;
    void displayMessage(std::string message);
    };
};
#endif