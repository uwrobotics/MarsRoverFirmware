#include "Logger.h"

namespace Utility {
static char buffer[4096];  // Can increase max buffer length if needed
}

int Utility::Logger::printf(const char* format, ...) {
  va_list arg;

  va_start(arg, format);
  int num_chars = std::vsprintf(buffer, format, arg);
  va_end(arg);

#if defined(UART_LOGGING)
  std::printf(buffer);
#endif

#if defined(SWO_LOGGING)
  char* c = buffer;

  while (*c) {
    ITM_SendChar(*c++);
    num_chars++;
  }
#endif
  return num_chars;
}

Utility::Logger& Utility::Logger::operator<<(long n) {
  return operator<<(static_cast<long long>(n));
}

Utility::Logger& Utility::Logger::operator<<(unsigned long n) {
  return operator<<(static_cast<unsigned long long>(n));
}

Utility::Logger& Utility::Logger::operator<<(bool n) {
  return operator<<(static_cast<long long>(n));
}

Utility::Logger& Utility::Logger::operator<<(short n) {
  return operator<<(static_cast<long long>(n));
}

Utility::Logger& Utility::Logger::operator<<(unsigned short n) {
  return operator<<(static_cast<unsigned long long>(n));
}

Utility::Logger& Utility::Logger::operator<<(int n) {
  return operator<<(static_cast<long long>(n));
}

Utility::Logger& Utility::Logger::operator<<(unsigned int n) {
  return operator<<(static_cast<unsigned long long>(n));
}

Utility::Logger& Utility::Logger::operator<<(long long n) {
#if defined(UART_LOGGING)
  std::printf("%lld", n);
#endif

#if defined(SWO_LOGGING)
  std::sprintf(buffer, "%lld", n);
  char* c = buffer;

  while (*c) {
    ITM_SendChar(*c++);
  }
#endif

  return *this;
}

Utility::Logger& Utility::Logger::operator<<(unsigned long long n) {
#if defined(UART_LOGGING)
  std::printf("%llu", n);
#endif

#if defined(SWO_LOGGING)
  std::sprintf(buffer, "%llu", n);
  char* c = buffer;

  while (*c) {
    ITM_SendChar(*c++);
  }
#endif

  return *this;
}

Utility::Logger& Utility::Logger::operator<<(double f) {
  return operator<<(static_cast<long double>(f));
}

Utility::Logger& Utility::Logger::operator<<(float f) {
  return operator<<(static_cast<long double>(f));
}

Utility::Logger& Utility::Logger::operator<<(long double f) {
#if defined(UART_LOGGING)
  std::printf("%Lf", f);
#endif

#if defined(SWO_LOGGING)
  std::sprintf(buffer, "%Lf", f);
  char* c = buffer;

  while (*c) {
    ITM_SendChar(*c++);
  }
#endif

  return *this;
}

Utility::Logger& Utility::Logger::operator<<(char c) {
#if defined(UART_LOGGING)
  std::printf("%c", c);
#endif

#if defined(SWO_LOGGING)
  ITM_SendChar(c);
#endif

  return *this;
}

Utility::Logger& Utility::Logger::operator<<(const char* format) {
#if defined(UART_LOGGING)
  std::printf(format);
#endif

#if defined(SWO_LOGGING)
  std::sprintf(buffer, format);
  char* c = buffer;

  while (*c) {
    ITM_SendChar(*c++);
  }
#endif

  return *this;
}
