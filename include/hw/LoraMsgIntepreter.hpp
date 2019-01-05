#include <vector>
#include "common/SanlaMessage.hpp"

namespace sanla {
    namespace hw_interfaces {

        using SanlaPacket = sanla::sanlamessage::SanlaPacket;

        class LoraMessageIntepreter {
            public:
            LoraMessageIntepreter();
            ~LoraMessageIntepreter();

            std::vector<char> buffer;

            bool CheckMessage(const char);
            SanlaPacket ValidateMessage(const char);
            char SerializePacket(SanlaPacket);

        };
    }; // hw_interfaces
}; // sanla