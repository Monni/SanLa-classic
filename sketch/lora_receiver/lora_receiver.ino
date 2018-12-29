#include <SPI.h>
#include <LoRa.h>

#define SCK     5    // GPIO5  -- SX1278's SCK
#define MISO    19   // GPIO19 -- SX1278's MISnO
#define MOSI    27   // GPIO27 -- SX1278's MOSI
#define SS      18   // GPIO18 -- SX1278's CS
#define RST     14   // GPIO14 -- SX1278's RESET
#define DI0     26   // GPIO26 -- SX1278's IRQ(Interrupt Request)
#define LED     2
#define BAND 868E6

void setup() {
  Serial.begin(115200);
  while (!Serial);

  Serial.println("LoRa Receiver");

  SPI.begin(SCK,MISO,MOSI,SS);
  LoRa.setPins(SS, RST, DI0);
  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  
  // Register receive callback
  LoRa.onReceive(onReceive);
  // Put the radio into receive mode
  LoRa.receive();
  pinMode(LED, OUTPUT);
}

void loop() {
  delay(2200);
  digitalWrite(LED, LOW);
   
  
}

void onReceive(int packetSize) {
  // Received a packed
  Serial.print("Received packet '");

  if (packetSize == 0) return;          // if there's no packet, return
  
  String incoming = "";                 // payload of packet
  while (LoRa.available()) {            // can't use readString() in callback, so
    incoming += (char)LoRa.read();      // add bytes one by one
  }
  digitalWrite(LED, HIGH);

  // print RSSI of packet
  Serial.print(incoming);
  Serial.print("' with RSSI ");
  Serial.println(LoRa.packetRssi());
}
