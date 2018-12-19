#include <SPI.h>
#include <LoRa.h>
#include <Arduino.h>
#include <string>

// LoRa HW definitions
#define SCK     5    // GPIO5  -- SX1278's SCK
#define MISO    19   // GPIO19 -- SX1278's MISnO
#define MOSI    27   // GPIO27 -- SX1278's MOSI
#define SS      18   // GPIO18 -- SX1278's CS
#define RST     14   // GPIO14 -- SX1278's RESET
#define DI0     26   // GPIO26 -- SX1278's IRQ(Interrupt Request)

#define BAND        868E6
#define TX_POWER    18      // TODO set to 20 after checking if sink needed.
#define S_FACTOR    12      // Supports values between 6 and 12.
#define SIG_BNDWDTH 125E3   // Supports values 7.8E3, 10.4E3, 15.6E3, 20.8E3, 31.25E3, 41.7E3, 62.5E3, 125E3, and 250E3
#define CODING_RATE 5       // Supports values between 5 and 8
#define PREAMBL_LEN 6
#define SYNC_WORD   0x12

#ifndef SANLA_CLASSIC_LORAMODULE_H
#define SANLA_CLASSIC_LORAMODULE_H

namespace sanla {
    namespace lora {

        class LoRaModule {
        public:
            LoRaModule();
            void begin();
            void sendMessage(String message);

        private:
            void setRadioParameters();
            void packetHeader();
            static void onMessage(int packetSize);
        };

    };
};


#endif //SANLA_CLASSIC_LORAMODULE_H
