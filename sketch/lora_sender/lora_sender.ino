#include <SPI.h>
#include <LoRa.h>

#define SCK     5    // GPIO5  -- SX1278's SCK
#define MISO    19   // GPIO19 -- SX1278's MISnO
#define MOSI    27   // GPIO27 -- SX1278's MOSI
#define SS      18   // GPIO18 -- SX1278's CS
#define RST     14   // GPIO14 -- SX1278's RESET
#define DI0     26   // GPIO26 -- SX1278's IRQ(Interrupt Request)
#define BAND 868E6

int counter = 0;

void setup() {
  
  Serial.begin(115200);
  while (!Serial);
  Serial.println("LoRa Sender");

  SPI.begin(SCK,MISO,MOSI,SS);
  LoRa.setPins(SS, RST, DI0);
  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
  Serial.print("Sending packet: ");
  Serial.println(counter);

  byte sender = 0xFF;
  String payload = "Foo walks into a bar baz qux moo";

  // send packet
  LoRa.beginPacket();
  LoRa.write(sender);
  LoRa.print(payload);
  LoRa.endPacket();

  counter++;

  delay(5000);
}
