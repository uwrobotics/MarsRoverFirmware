#include <string>

#include "CANMsg.h"
#include "LookupTable.h"
#include "hw_bridge.h"
#include "mbed.h"

mbed_error_status_t trivial1(CANMsg& msg);
mbed_error_status_t trivial2(CANMsg& msg);

int main() {
  printf("\n LookupTable Test App Started \r\n");
  printf("\r\n####### HOW TO ITERATE #######\r\n");
  printf("Note: Won't be in order when iterating since based off hash-map. ");
  printf("What's important is that key-value pairs match\r\n");
  const lookup_table::LookupTable<std::string, int> week = {
      {"Monday", 1}, {"Tuesday", 2}, {"Wednesday", 3}, {"Thursday", 4}, {"Friday", 5}, {"Saturday", 6}, {"Sunday", 7}};
  // iterators:
  for (auto it = week.begin(); it != week.end(); it++) {
    printf("%s %d\r\n", it->first.c_str(), it->second);
  }
  printf("\r\n");
  // range-based for loop
  for (const auto& i : week) {
    printf("%s %d\r\n", i.first.c_str(), i.second);
  }
  printf("\r\n");
  // structured bindings
  for (const auto& [key, value] : week) {
    printf("%s %d\r\n", key.c_str(), value);
  }

  printf("\r\n####### HOW TO INDEX #######\r\n");
  // No default case
  const lookup_table::LookupTable<std::string, int> a = {{"Jan", 1}, {"Feb", 2}};
  printf("Existing Key: %d\r\n", a["Jan"].value_or(-1));
  printf("Existing Key: %d\r\n", a["Feb"].value_or(-1));
  printf("Non-Existing Key: %d\r\n", a["Doesnt Exist"].value_or(-1));
  printf("\r\n");

  // Default case: Note necessary cast around the default
  const lookup_table::LookupTable<uint8_t, uint8_t, static_cast<uint8_t>(42)> b = {{0, 100}, {1, 101}};
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

  while (true) {
  }
}

mbed_error_status_t trivial1(CANMsg& msg) {
  printf("Hello from trivial1\r\n");
  return MBED_SUCCESS;
}
mbed_error_status_t trivial2(CANMsg& msg) {
  printf("Hello from trivial2\r\n");
  return MBED_SUCCESS;
}
