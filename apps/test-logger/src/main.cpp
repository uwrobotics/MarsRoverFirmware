#include "Logger.h"
#include "mbed.h"

Utility::Logger logger;

int main() {
  Utility::Logger::printf("Logger %s%c\n", "Test", '!');

  logger << "Long: " << ((long)-58748397) << "\n";
  logger << "Unsigned long: " << ((unsigned long)123478421) << "\n";
  logger << "Bool: " << true << "\n";
  logger << "Short: " << ((short)-4) << "\n";
  logger << "Unsigned short: " << ((unsigned short)4) << "\n";
  logger << "Int: " << ((int)-4235) << "\n";
  logger << "Unsigned int: " << ((unsigned int)12434) << "\n";
  logger << "Long long: " << ((long long)8765456787654) << "\n";
  logger << "Double: " << ((double)234.56324546) << "\n";
  logger << "Float: " << ((float)-431.6543) << "\n";
  logger << "Long double: " << ((long double)2345453.98765) << "\n";
  logger << "Char: " << 'a' << "\n";
  logger << "String: "
         << "hello logger"
         << "\n";

  // Test error logging
  MBED_ASSERT(false);

  while (true)
    ;
}
