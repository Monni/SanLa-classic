#include <vector>
#include "common/SanlaMessage.hpp"

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