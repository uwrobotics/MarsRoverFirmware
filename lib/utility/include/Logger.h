#pragma once

namespace Utility {

class Logger {
 public:
  // C-style printf
  static int printf(const char* format, ...);

  Logger& operator<<(long n);
  Logger& operator<<(unsigned long n);
  Logger& operator<<(bool n);
  Logger& operator<<(short n);
  Logger& operator<<(unsigned short n);
  Logger& operator<<(int n);
  Logger& operator<<(unsigned int n);
  Logger& operator<<(long long n);
  Logger& operator<<(unsigned long long n);

  Logger& operator<<(double f);
  Logger& operator<<(float f);
  Logger& operator<<(long double f);

  Logger& operator<<(char c);
  Logger& operator<<(const char* format);
};

}  // namespace Utility
