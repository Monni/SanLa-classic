
namespace sanla {
    namespace sanlamessage {
        namespace {
            using SanlaPacket = sanla::sanlamessage::SanlaPacket;

            struct MessageActionStruct {
                void (*drop)(SanlaPacket);
                bool (*store)(SanlaPacket);
                void (*respond)(SanlaPacket);
            };
        }; // anonymous

        class MessageParser {
            public:
            MessageParser() = delete;
            MessageParser(void (*drop_func)(SanlaPacket),
                        bool (*store_func)(SanlaPacket), 
                        void (*respond_func)(SanlaPacket));
            ~MessageParser();

            void ParseMessage(SanlaPacket);

            private:
            MessageActionStruct actions;
        };
    }; // sanlamessage
}; // sanla