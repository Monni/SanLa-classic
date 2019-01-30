#ifndef SANLACLASSIC_COMMON_SANLAMESSAGE_H_
#define SANLACLASSIC_COMMON_SANLAMESSAGE_H_

#include <stdint.h>
#include <string>
#include <cstddef>
#include <sstream>
#include "constants.hpp"

namespace sanla {

    using PackageId_t = uint32_t;
    using PayloadSeq_t = uint8_t;
    using PayloadLength_t = uint16_t;
    using SenderId_t = uint16_t;
    using RecipientId_t = uint16_t;
    using PayloadChecksum_t = uint32_t;
    using flag_t = u_char;
    using payload_t = char[lora::MAX_PACKET_PAYLOAD_SIZE];


    class Serializable
    {
    public:
        Serializable(){}
        virtual ~Serializable(){}

        virtual void serialize(std::stringstream& stream) = 0;
    };

    namespace sanlamessage {
        const uint8_t RECIPIENT_ID_MAX_SIZE {16};
        const flag_t BRO {0x1}, REQ {0x2}, PRO {0x3}, PAC {0x4}, ACK {0x8},
        SEN {0xC}, RES {0xA}, PACREQ {0x6}, PACPRO {0x7}, PROACK {0xB},
        PACSEN {0xF}, PACRES {0xD};

        struct SanlaPacket {
            flag_t flags;
            PackageId_t package_id;
            SenderId_t sender_id;
            char recipient_id[RECIPIENT_ID_MAX_SIZE];
            PayloadLength_t package_payload_length;
            uint8_t payload_seq;
            PayloadChecksum_t payload_chks;
            payload_t payload;
        };
    };

    struct MessageBody {
        char* sender; // TODO: use [max_size] instead of pointer, also define max_size
        const char *payload;
    };

    struct MessageHeader : public Serializable {
        flag_t flags;
        uint8_t payload_seq;
        PayloadLength_t length;
        SenderId_t sender_id;
        PayloadChecksum_t payload_chks;
        PackageId_t package_id;
        std::string recipient_id;

        virtual void serialize(std::stringstream& stream) {
            stream << flags << payload_seq << length << sender_id << payload_chks << package_id << recipient_id;
        }

    };

    class SanlaMessagePackage {
        MessageHeader header;
        MessageBody body;

        public:
        SanlaMessagePackage(u_char, uint8_t, uint16_t, 
        uint16_t, long, uint32_t, std::string, MessageBody);
        SanlaMessagePackage(MessageHeader, MessageBody);
        // This class is not moveable
        SanlaMessagePackage(SanlaMessagePackage&&) = delete;
        SanlaMessagePackage& operator=(SanlaMessagePackage&&) = delete;
        // This class is not copyable
        SanlaMessagePackage(const SanlaMessagePackage&) = delete;
        SanlaMessagePackage& operator=(const SanlaMessagePackage&) = delete;

        ~SanlaMessagePackage(){};

        uint8_t GetPackageLength();
        uint16_t GetTotalPackageLength();
        MessageHeader& GetPackageHeader();
        MessageBody& GetPackageBody();
    };
};
#endif