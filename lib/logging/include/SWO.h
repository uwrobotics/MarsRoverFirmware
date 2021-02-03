#pragma once

namespace Logger {

// C-style printf - prints to SWO interface
void SWO_printf(const char* format, ...);

// SWO logger
class SWO {
 public:
  SWO& operator<<(long n);
  SWO& operator<<(unsigned long n);
  SWO& operator<<(bool n);
  SWO& operator<<(short n);
  SWO& operator<<(unsigned short n);
  SWO& operator<<(int n);
  SWO& operator<<(unsigned int n);
  SWO& operator<<(long long n);
  SWO& operator<<(unsigned long long n);

  SWO& operator<<(double f);
  SWO& operator<<(float f);
  SWO& operator<<(long double f);

  SWO& operator<<(char c);
  SWO& operator<<(const char* format);
};

}  // namespace Logger
