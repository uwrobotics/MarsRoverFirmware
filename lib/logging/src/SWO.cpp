#include "SWO.h"

constexpr uint32_t MAX_BUFFER_LEN = 4096;

static char buffer[MAX_BUFFER_LEN];

void Logger::SWO_printf(const char* format, ...) {
  va_list arg;

  va_start(arg, format);
  sprintf(buffer, format, arg, 0);
  va_end(arg);

  char* c = buffer;

  while (*c) {
    ITM_SendChar(*c++);
  }
}

Logger::SWO& Logger::SWO::operator<<(long n) {
  return operator<<((long long)n);
}

Logger::SWO& Logger::SWO::operator<<(unsigned long n) {
  return operator<<((unsigned long long)n);
}

Logger::SWO& Logger::SWO::operator<<(bool n) {
  return operator<<((long long)n);
}

Logger::SWO& Logger::SWO::operator<<(short n) {
  return operator<<((long long)n);
}

Logger::SWO& Logger::SWO::operator<<(unsigned short n) {
  return operator<<((unsigned long long)n);
}

Logger::SWO& Logger::SWO::operator<<(int n) {
  return operator<<((long long)n);
}

Logger::SWO& Logger::SWO::operator<<(unsigned int n) {
  return operator<<((unsigned long long)n);
}

Logger::SWO& Logger::SWO::operator<<(long long n) {
  sprintf(buffer, "%lld", n);
  char* c = buffer;

  while (*c) {
    ITM_SendChar(*c++);
  }

  return *this;
}

Logger::SWO& Logger::SWO::operator<<(unsigned long long n) {
  sprintf(buffer, "%llu", n);
  char* c = buffer;

  while (*c) {
    ITM_SendChar(*c++);
  }

  return *this;
}

Logger::SWO& Logger::SWO::operator<<(double f) {
  return operator<<((long double)f);
}

Logger::SWO& Logger::SWO::operator<<(float f) {
  return operator<<((long double)f);
}

Logger::SWO& Logger::SWO::operator<<(long double f) {
  sprintf(buffer, "%Lf", f);
  char* c = buffer;

  while (*c) {
    ITM_SendChar(*c++);
  }

  return *this;
}

Logger::SWO& Logger::SWO::operator<<(char c) {
  ITM_SendChar(c);
  return *this;
}

Logger::SWO& Logger::SWO::operator<<(const char* format) {
  sprintf(buffer, format);
  char* c = buffer;

  while (*c) {
    ITM_SendChar(*c++);
  }

  return *this;
}
