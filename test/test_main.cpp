#include <Arduino.h>
#include <unity.h>

// void setUp(void) {
// // set stuff up here
// }

// void tearDown(void) {
// // clean stuff up here
// }

#include "common/SanlaMessage.hpp"

using namespace sanla;

const u_char PACSEN = sanlamessage::PACSEN;

void test_package_constructor(void) {
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
    // NOTE!!! Wait for >2 secs
    // if board doesn't support software reset via Serial.DTR/RTS
    delay(2000);

    UNITY_BEGIN();    // IMPORTANT LINE!
    RUN_TEST(test_package_constructor);

    UNITY_END(); // stop unit testing
}
