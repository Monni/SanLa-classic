# TCP Multipart SanlaMessage

## Glossary

| Word | Explanation |
| --- | --- |
Package | Consists of multiple packets 

Multipart message would allow for a sending of larger than 100 byte packages. 
100 bytes is the reliable max size for LoRa message, however 50 bytes is the recommended size. 
Messages longer than that can be unreliable.
In our case we would want to possibly send a message with a body of 2000 characters (16kB).

__Multipart package flow__

![alt text](https://raw.githubusercontent.com/Monni/SanLa-classic/docs/docs/specifications/initial_protocol_plan.png?token=AKDudAex8yQ0V2Px4xhSDnImX0EduHL4ks5cElT5wA%3D%3D "Packet flow")

NOTE: there can be multiple senders and receivers

## Messages

| Message name | description |
| --- | --- |
| ACK | OK response to received hash value |
| PACKET_BROADCAST | A message offering a first part of a packet group |
| PACKET_REQUEST | Response to PACKET_BROADCAST |
| PACKET_OFFER | Response to PACKET_REQUEST |
| PACKET_RECV | Response sent to sender after receiving whole message|
| NACK | Not OK response to received hash value |
| MSG_PART | Part of message |

### ACK

### PACKET_BROADCAST
 
### PACKET_OFFER

### PACKET_RECV

### PACKET_REQUEST

### NACK

### MSG_PART


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
