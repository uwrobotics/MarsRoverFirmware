#include "Logger.h"
#include "mbed.h"

int main() {
  printf("Logger %s%c\r\n", "Test", '!');

  Utility::logger << "Long: " << ((long)-58748397) << "\r\n";
  Utility::logger << "Unsigned long: " << ((unsigned long)123478421) << "\r\n";
  Utility::logger << "Bool: " << true << "\r\n";
  Utility::logger << "Short: " << ((short)-4) << "\r\n";
  Utility::logger << "Unsigned short: " << ((unsigned short)4) << "\r\n";
  Utility::logger << "Int: " << ((int)-4235) << "\r\n";
  Utility::logger << "Unsigned int: " << ((unsigned int)12434) << "\r\n";
  Utility::logger << "Long long: " << ((long long)8765456787654) << "\r\n";
  Utility::logger << "Double: " << ((double)234.56324546) << "\r\n";
  Utility::logger << "Float: " << ((float)-431.6543) << "\r\n";
  Utility::logger << "Long double: " << ((long double)2345453.98765) << "\r\n";
  Utility::logger << "Char: " << 'a' << "\r\n";
  Utility::logger << "String: "
                  << "hello logger"
                  << "\r\n";

  // Test mbed error log
  MBED_ASSERT(false);

  while (true)
    ;
}
