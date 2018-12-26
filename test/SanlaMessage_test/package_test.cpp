#include <Arduino.h>
#include <unity.h>

#include "common/SanlaMessage.hpp"

using namespace sanla;

const u_char PACSEN = sanlamessage::PACSEN;

void test_package_constructor() {
    char* sender = "Viljami\0";
    char* payload = "Oispa kaljaa\0";
    sanla::MessageHeader target_header {.flags = 1};
    sanla::MessageBody target_body {};
    SanlaMessagePackage package{{
        PACSEN, 1, 2000, 0x680e90e47a7e, 0xa00d713ef738, 1337, "make"},
        {sender, payload}};

    TEST_ASSERT_EQUAL(target_header.flags, package.GetPackageHeader().flags);
}
void setup() {
    delay(2000);

    UNITY_BEGIN();
    RUN_TEST(test_package_constructor);
    UNITY_END();
}

void loop() {

}