# Sanla Message

This document explains the correct format for a Sanla message. Message consists of two parts: header and body. Header consists of identifying information and body contains the message itself.
## Header

Header consists of sender identifier, receiver's identifier, message length, and a unix time stamp. Receiver's id is a base64 encoded value of group name and a pre-shared password.

**Header elements**

| name | description | type |
| --- | --- | --- |
| from | sender | string |
| to | recipient | string |
| length | message-length | int |
| time | timestamp | long |

In JSON the example message header would look like this
```json
{
    from: 'SenderID',
    to: 'RecipientID',
    length: 2000,
    time: 1543850646
}
```
## Body

Body consists of a single element, message-content, which can contain 2000 characters.

**Body elements**

| name | description | type |
| --- | --- | --- |
| msg | message content | string |

In JSON the example message would look like this.
```json
{
    msg: 'A really long message....'
}
```
