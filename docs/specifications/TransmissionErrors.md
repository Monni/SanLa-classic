# Transmission Errors

This document provides possible error cases on each state of a transmission and explains how the protocol handles them.

## BRO

Package is being broadcasted to nearby devices.

**Identified error cases**
* No listening devices in range.
* Listening device receives a distorted packet

### Sequence diagram
![alt text][err_bro]

#### Steps
1. Person sends a message package.
2. Device broadcasts the message, without any listening device to successfully reveive the message sent.
..* Logic will be the same for both identified error cases.
3. Validate if remaining Time To Live exists and reduce.
..* Loop steps 2 and 3 until Time To Live reaches zero.
4. Display error message.
5. Destroy package.

[err_bro]: https://github.com/Monni/SanLa-classic/blob/docs/docs/sequence_diagrams/error_cases/err_bro.png "Error in BRO state"