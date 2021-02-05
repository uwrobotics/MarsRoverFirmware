#include "Logger.h"

#define SWO_Logging // TODO: need to configure this at the target level

static char buffer[4096]; // Can increase max buffer length if needed

void Utility::Logger::printf(const char* format, ...) {
  va_list arg;

  va_start(arg, format);
  sprintf(buffer, format, arg, 0);
  va_end(arg);

#if defined(UART_Logging)
  printf(buffer);
#endif
#if defined(SWO_Logging)
  char* c = buffer;
  while (*c) {
    ITM_SendChar(*c++);
  }
#endif
}

Utility::Logger& Utility::Logger::operator<<(long n) {
  return operator<<((long long)n);
}

Utility::Logger& Utility::Logger::operator<<(unsigned long n) {
  return operator<<((unsigned long long)n);
}

Utility::Logger& Utility::Logger::operator<<(bool n) {
  return operator<<((long long)n);
}

Utility::Logger& Utility::Logger::operator<<(short n) {
  return operator<<((long long)n);
}

Utility::Logger& Utility::Logger::operator<<(unsigned short n) {
  return operator<<((unsigned long long)n);
}

Utility::Logger& Utility::Logger::operator<<(int n) {
  return operator<<((long long)n);
}

Utility::Logger& Utility::Logger::operator<<(unsigned int n) {
  return operator<<((unsigned long long)n);
}

Utility::Logger& Utility::Logger::operator<<(long long n) {
#if defined(UART_Logging)
  printf("%lld", n);
#endif
#if defined(SWO_Logging)
  sprintf(buffer, "%lld", n);
  char* c = buffer;

  while (*c) {
    ITM_SendChar(*c++);
  }
#endif
  return *this;
}

Utility::Logger& Utility::Logger::operator<<(unsigned long long n) {
#if defined(UART_Logging)
  printf("%llu", n);
#endif
#if defined(SWO_Logging)
  sprintf(buffer, "%llu", n);
  char* c = buffer;

  while (*c) {
    ITM_SendChar(*c++);
  }
#endif
  return *this;
}

Utility::Logger& Utility::Logger::operator<<(double f) {
  return operator<<((long double)f);
}

Utility::Logger& Utility::Logger::operator<<(float f) {
  return operator<<((long double)f);
}

Utility::Logger& Utility::Logger::operator<<(long double f) {
#if defined(UART_Logging)
  printf("%Lf", f);
#endif
#if defined(SWO_Logging)
  sprintf(buffer, "%Lf", f);
  char* c = buffer;

  while (*c) {
    ITM_SendChar(*c++);
  }
#endif
  return *this;
}

Utility::Logger& Utility::Logger::operator<<(char c) {
#if defined(UART_Logging)
  printf("%c", c);
#endif
#if defined(SWO_Logging)
  ITM_SendChar(c);
#endif
  return *this;
}

Utility::Logger& Utility::Logger::operator<<(const char* format) {
#if defined(UART_Logging)
  printf(format);
#endif
#if defined(SWO_Logging)
  sprintf(buffer, format);
  char* c = buffer;

  while (*c) {
    ITM_SendChar(*c++);
  }
#endif
  return *this;
}
