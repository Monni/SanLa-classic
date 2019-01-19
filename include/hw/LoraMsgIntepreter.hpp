#include <vector>
#include "common/SanlaMessage.hpp"

#ifndef SANLACLASSIC_HW_LORAMSGINTEPRETER_H_
#define SANLACLASSIC_HW_LORAMSGINTEPRETER_H_

namespace sanla {
    namespace hw_interfaces {

        using SanlaPacket = sanla::sanlamessage::SanlaPacket;

        class LoraMessageIntepreter {
            public:
            LoraMessageIntepreter();
            ~LoraMessageIntepreter();

            std::vector<u_char> buffer;

            bool CheckMessage(const u_char);
            SanlaPacket ValidateMessage(const u_char);
            u_char SerializePacket(SanlaPacket);

        };
    }; // hw_interfaces
}; // sanla
#endif