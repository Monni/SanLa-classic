# LoRaModule
Arduino-LoRa API customised for SanLa specific use cases

## Description
This API works as a middleware for communication with LoRa hardware.
The actual communication between ESP32 board and LoRa hardware is handled by Arduino-LoRa by sandeepmistry.
For more information of Arduino-LoRa, visit: [https://github.com/sandeepmistry/arduino-LoRa](https://github.com/sandeepmistry/arduino-LoRa)

## Features
API supports following functionalities

### Setup
Begin by instantiating LoRaModule class and calling `begin`
```
sanla::lora::LoRaModule lora;
lora.begin();
```
Function `begin` will set the following:
- Pins used by LoRa hardware module
- Tx Power
- Spreading Factor
- Signal Bandwidth
- Coding Rate
- Preamble length
- Sync Word

To be continued..
