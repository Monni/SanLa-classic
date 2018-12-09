# Transmission Errors

This document provides possible error cases on each state of a transmission and explains how the protocol handles them.

## BRO

Package is being broadcasted to nearby devices.

### Error Case: No listening devices in range or listening device receives a distorted packet.

A device broadcasts available package but no listening devices are in range of the transmission or listening device receives a distorted packet.

### Sequence Diagram
![alt text][err_bro]

#### Steps
1. Person sends a message package.
2. Device broadcasts the message, without any listening device to successfully reveive the sent message.
3. Validate if remaining Time To Live exists and reduce.
    * Repeat steps 2 and 3 until Time To Live reaches zero.
4. Display error message.
5. Destroy package.

## PAC-REQ

Packet is being requested from nearby devices.

**TODO:**
* Listening device receives a distorted packet.
    * PAC-REQ does not contain payload, how to handle?
* Listening device does not have requested packet.
    * Is not an actual error, many devices will not have a packet.

### Error case: No listening devices in range

A device requests around for a single packet belonging to a certain package, but no listening devices are in range of the transmission.

#### Sequence Diagram
![alt text][err_pac_req]

#### Steps
1. Device requests a packet, without any listening device in range.
2. Validate if Time To Live remaining and reduce by one.
    * Repeat steps 1 and 2 until Time To Live reaches zero.
3. Destroy and forget received package.

TODO:
4. Blacklist package id to not care next time when broadcasted?

[err_bro]: https://github.com/Monni/SanLa-classic/blob/docs/docs/sequence_diagrams/error_cases/err_bro.png "Error in BRO state"
[err_pac_req]: https://github.com/Monni/SanLa-classic/blob/docs/docs/sequence_diagrams/error_cases/err_pac_req.png "Error in PAC-REQ state"