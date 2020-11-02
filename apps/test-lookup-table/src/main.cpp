#include "LookupTable.h"
#include <string>
#include "mbed.h"
#include "CANMsg.h"
#include "hw_bridge.h"

mbed_error_status_t trivial1(CANMsg& msg);
mbed_error_status_t trivial2(CANMsg& msg);

int main() {
    printf("\n LookupTable Test App Started \r\n");
    // No default case
    const LookupTable<std::string, int> a = {{"Jan", 1}, {"Feb", 2}};
    printf("Existing Key: %d\r\n", a["Jan"].value_or(-1));
    printf("Existing Key: %d\r\n", a["Feb"].value_or(-1));
    printf("Non-Existing Key: %d\r\n", a["Doesnt Exist"].value_or(-1));
    printf("\r\n");

    // Default case: Note necessary cast around the default
    const LookupTable<uint8_t, uint8_t, static_cast<uint8_t>(42)> b = {{0, 100}, {1, 101}};
    printf("Existing Key: %d\r\n", b[0]);
    printf("Existing Key: %d\r\n", b[1]);
    printf("Non-Existing Key: %d\r\n", b[2]);
    printf("\r\n");

    // Function pointer example
    const CANMsg::CANMsgHandlerMap c = {{HWBRIDGE::CANID::TPDO1, trivial1}, {HWBRIDGE::CANID::TPDO2, trivial2}};
    CANMsg msg;
    printf("Existing Key: \r\n");
    c[HWBRIDGE::CANID::TPDO1](msg);
    printf("Existing Key: \r\n");
    c[HWBRIDGE::CANID::TPDO2](msg);
    printf("Non-Existing Key \r\nNOTE THE TERMINATION OF THE PROGRAM: \r\n");
    c[HWBRIDGE::CANID::TPDO3](msg);

    while(true);
}

mbed_error_status_t trivial1(CANMsg& msg) {
    printf("Hello from trivial1\r\n");
    return MBED_SUCCESS;
}
mbed_error_status_t trivial2(CANMsg& msg) {
    printf("Hello from trivial2\r\n");
    return MBED_SUCCESS;
}