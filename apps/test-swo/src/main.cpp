#include "SWO.h"
#include "mbed.h"

Logger::SWO swo;

int main() {
  Logger::SWO_printf("SWO logging test\n");

  swo << "Long: " << ((long)-58748397) << "\n";
  swo << "Unsigned long: " << ((unsigned long)123478421) << "\n";
  swo << "Bool: " << true << "\n";
  swo << "Short: " << ((short)-4) << "\n";
  swo << "Unsigned short: " << ((unsigned short)4) << "\n";
  swo << "Int: " << ((int)-4235) << "\n";
  swo << "Unsigned int: " << ((unsigned int)12434) << "\n";
  swo << "Long long: " << ((long long)8765456787654) << "\n";
  swo << "Double: " << ((double)234.56324546) << "\n";
  swo << "Float: " << ((float)-431.6543) << "\n";
  swo << "Long double: " << ((long double)2345453.98765) << "\n";
  swo << "Char: " << 'a' << "\n";
  swo << "String: "
      << "hello SWO"
      << "\n";

  while (true) {
  }
}
