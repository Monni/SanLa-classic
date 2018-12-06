# Multipart SanlaMessage

## Glossary

| Word | Explanation |
| --- | --- |
Package | Consists of multiple packets 

## Packet 

Multipart message would allow for a sending of larger than 100 byte packages. 
100 bytes is the reliable max size for LoRa message, however 50 bytes is the recommended size. 
Messages longer than that can be unreliable.
In our case we would want to possibly send a message with a body of 2000 characters (16kB).

__Multipart package flow__

![alt text](https://raw.githubusercontent.com/Monni/SanLa-classic/docs/docs/specifications/initial_protocol_plan.png?token=AKDudAex8yQ0V2Px4xhSDnImX0EduHL4ks5cElT5wA%3D%3D "Packet flow")

NOTE: there can be multiple senders and receivers

__Packet Type Flags__

| Hex value | Type name | Name | Description |
| --- | --- | --- | --- |
0x1 | BRO | Broadcast
0x2 | REQ | Request
0x3 | PRO | Propsition
0x4 | PAC | Packet
0x8 | ACK | Acknowledgement
0xC | SEN | Send
0xA | RES | Reset
0x6 | PACREQ | Packet Request
0x7 | PACPRO | Packet Proposition
0xB | PROACK | Proposition Acknowledgement
0xF | PACSEN | Packet Send
0xD | PACRES | Packet Reset

## Packet Messages

__Field types__

| Field | Type | Width (in bits) | Notes
| --- | --- | --- | --- |
| Flags | uint8_t | 8
| PackageID | long | 32
| SenderID | uint16_t | 16 |
| RecipientID | char | 49 |  max name length can be 16 characters long
| PackagePayloadLength | uint16_t | 16
| PayloadChks | long | 32
| PayloadSequence | uint8_t | 8
| Payload | char | rest of the available space in packet

## Broadcast BRO

- Flags
- PackageID
- SenderID
- RecipientID
- PackagePayloadLength

### Packet Request PACREQ

- Flags
- PackageID
- SenderID
- PayloadSequence

### Packet Proposition PACPRO

- Flags
- PackageID
- SenderID
- PayloadSequence

### Proposition Acknowledgement PROACK

- Flags
- PackageID
- SenderID
- PayloadSequence

### Packet Send PACSEN

- Flags
- PackageID
- SenderID
- PayloadSequence
- Payload
- PayloadChks

### Packet Request

- Flags
- PackageID
- SenderID
- PayloadSequence
