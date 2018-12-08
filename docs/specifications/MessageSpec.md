# Sanla Message

This document explains the correct format for a Sanla message. Message consists of two parts: header and body. Header consists of identifying information and body contains the message itself.
## Header

Header consists of sender identifier, receiver's identifier, message length, and a unix time stamp. Receiver's id is a base64 encoded value of group name and a pre-shared password.

**Header elements**

| name | description | type | note |
| --- | --- | --- | --- |
| Flags | Identifies which transport method is in use | uint8_t | TODO: need a flag for fully constructed package? |
| Package ID | Identifier to which package all packets belongs to | long ||
| Sender ID | Sender machine id | uint16_t ||
| Recipient ID | Recipient group id| char | TODO: To keep recipient groups with same name unique, do some hacky wacky hash('group name' + 'password') ? Also need to define max length.|
| length | Payload length | uint16_t ||
| PayloadChks | Checksum for payload validation | long ||

| PayloadSeq | Identifies payload position in a package | uint8_t ||
## Body

Body consists of a single element, message-content, which can contain 2000 characters.

**Body elements**

| name | description | type |
| --- | --- | --- |
| Sender | Human readable sender name (person) | char |
| Payload | Message payload | char |

JSON example of a fully constructed package with header and body:
```json
{
	'data': {
		'header': {
			'flags': 0x8,
			'package_id': 12345678123456781234567812345678,
			'sender_id': 1234567812345678,
			'recipient_id': 'flj4390fk34k3ofö-l4-r0943k',
			'length': 2000,
			'payload_chks': '595f44fec1e92a71d3e9e77456ba80d1',
			'payload_seq': 2000
		},
		'body' : {
			'sender': 'Jaakko',
			'payload': 'A foo walks into a bar baz qux moo.'
		}
	}
	
	
    
}
```