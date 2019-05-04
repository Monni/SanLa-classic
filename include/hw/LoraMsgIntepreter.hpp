#ifndef SANLACLASSIC_HW_LORAMSGINTEPRETER_H_
#define SANLACLASSIC_HW_LORAMSGINTEPRETER_H_

#include <vector>
#include "common/SanlaPacket.hpp"

namespace sanla {
    namespace hw_interfaces {

        using SanlaPacket = sanla::messaging::SanlaPacket;

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