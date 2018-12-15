#include <Arduino.h>

#include "lora_sender.hpp"

int counter = 0;

void setup() {
  
  Serial.begin(115200);
  while (!Serial);
  Serial.println("LoRa Sender");

  SPI.begin(SCK,MISO,MOSI,SS);
  LoRa.setPins(SS, RST, DI0);
  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    Serial.println(LoRa.begin(866E6));
    while (1);
  }
}

void loop() {
  Serial.print("Sending packet: ");
  Serial.println(counter);

  // send packet
  LoRa.beginPacket();
  LoRa.print("hello ");
  LoRa.print(counter);
  LoRa.endPacket();

  counter++;

  delay(5000);
}
