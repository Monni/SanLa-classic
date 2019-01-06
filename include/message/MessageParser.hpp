#include "common/SanlaMessage.hpp"

namespace sanla {
    namespace sanlamessage {
        namespace {
            using SanlaPacket = sanla::sanlamessage::SanlaPacket;
            typedef void (*FuncPtr)(SanlaPacket);
        }; // anonymous

        struct MessageActionStruct {
                void (*drop)(SanlaPacket);
                void (*store)(SanlaPacket);
                void (*respond)(SanlaPacket);
            };

        class MessageParser {
            public:
            MessageParser();
            ~MessageParser();

            void setDropAction(FuncPtr);
            void setStoreAction(FuncPtr);
            void setRespondAction(FuncPtr);

            FuncPtr ParseMessage(SanlaPacket);

            private:
            MessageActionStruct actions;
            bool initCompelete = false;
        };
    }; // sanlamessage
}; // sanla