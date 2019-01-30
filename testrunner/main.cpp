#include <iostream>
#include "common/SanlaMessage.hpp"

int main() {
    sanla::MessageHeader head{};
    head.flags = 0x1;
    std::cout << "Hello world " << head.flags;
    std::cout << "BB";
}