# TCP Multipart SanlaMessage

Multipart message would allow for a sending of larger than 100 byte packages . 
100 bytes is the recommended max size of LoRa message. 
Messages longer than that can be unreliable.
In our case we would want to possibly send a message with a body of 2000 characters (16kB).

Flow of multipart message would look something like this.

1. Sender broadcasts (PACKET_BROADCAST) offering a group of packages with id of X in N parts.
1. Recipient sends an PACKET_REQUEST message, requesting the first packet of packet group X
1. Sender responds to PACKET_REQUEST with PACKET_OFFER
1. Receiver requests first part of X from the Sender.
1. Sender sends the MSG_PART
1. Recipient receives the MSG_PART
1. Recipient calculates the MSG_PART hash and sends it to Sender
1. Sender receives the calculated hash and sends ACK to Recipient if received hash was valid, otherwise sends NACK

Steps 2 - 8 are repeated repeated for each part of the message. When Recipient receives the final part it sends PACKET_RECV signal to Sender.

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