#include "Logger.h"

#include <inttypes.h>

using namespace Utility;

static char buffer[4096];  // Can increase max buffer length if needed

#if defined(SWO_LOGGING)
static int swo_printf(const char* format, ...);  // Only print to SWO (helper for mbed error hook)
#endif

int Logger::printf(const char* format, ...) {
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

Logger& Logger::operator<<(long n) {
  return operator<<(static_cast<long long>(n));
}

Logger& Logger::operator<<(unsigned long n) {
  return operator<<(static_cast<unsigned long long>(n));
}

Logger& Logger::operator<<(bool n) {
  return operator<<(static_cast<long long>(n));
}

Logger& Logger::operator<<(short n) {
  return operator<<(static_cast<long long>(n));
}

Logger& Logger::operator<<(unsigned short n) {
  return operator<<(static_cast<unsigned long long>(n));
}

Logger& Logger::operator<<(int n) {
  return operator<<(static_cast<long long>(n));
}

Logger& Logger::operator<<(unsigned int n) {
  return operator<<(static_cast<unsigned long long>(n));
}

Logger& Logger::operator<<(long long n) {
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

Logger& Logger::operator<<(unsigned long long n) {
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

Logger& Logger::operator<<(double f) {
  return operator<<(static_cast<long double>(f));
}

Logger& Logger::operator<<(float f) {
  return operator<<(static_cast<long double>(f));
}

Logger& Logger::operator<<(long double f) {
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

Logger& Logger::operator<<(char c) {
#if defined(UART_LOGGING)
  std::printf("%c", c);
#endif

#if defined(SWO_LOGGING)
  ITM_SendChar(c);
#endif

  return *this;
}

Logger& Logger::operator<<(const char* format) {
  Logger::printf(format);
  return *this;
}

#if defined(SWO_LOGGING)
int swo_printf(const char* format, ...) {
  va_list arg;

  va_start(arg, format);
  int num_chars = std::vsprintf(buffer, format, arg);
  va_end(arg);

  char* c = buffer;

  while (*c) {
    ITM_SendChar(*c++);
    num_chars++;
  }

  return num_chars;
}

// Application callback hook for mbed error events, only needed for SWO since mbed already logs through UART to stderr
// Partially copied from print_error_report() from mbed_error.c
void mbed_error_hook(const mbed_error_ctx* error_context) {
  int error_code   = MBED_GET_ERROR_CODE(error_context->error_status);
  int error_module = MBED_GET_ERROR_MODULE(error_context->error_status);

  swo_printf("\n\n++ MbedOS Error Info ++\nError Status: 0x%X Code: %d Module: %d\nError Message: ",
             error_context->error_status, error_code, error_module);

  switch (error_code) {
    // These are errors reported by kernel handled from mbed_rtx_handlers
    case MBED_ERROR_CODE_RTOS_EVENT:
      swo_printf("Kernel Error: 0x%" PRIX32 ", ", error_context->error_value);
      break;

    case MBED_ERROR_CODE_RTOS_THREAD_EVENT:
      swo_printf("Thread: 0x%" PRIX32 ", ", error_context->error_value);
      break;

    case MBED_ERROR_CODE_RTOS_MUTEX_EVENT:
      swo_printf("Mutex: 0x%" PRIX32 ", ", error_context->error_value);
      break;

    case MBED_ERROR_CODE_RTOS_SEMAPHORE_EVENT:
      swo_printf("Semaphore: 0x%" PRIX32 ", ", error_context->error_value);
      break;

    case MBED_ERROR_CODE_RTOS_MEMORY_POOL_EVENT:
      swo_printf("MemoryPool: 0x%" PRIX32 ", ", error_context->error_value);
      break;

    case MBED_ERROR_CODE_RTOS_EVENT_FLAGS_EVENT:
      swo_printf("EventFlags: 0x%" PRIX32 ", ", error_context->error_value);
      break;

    case MBED_ERROR_CODE_RTOS_TIMER_EVENT:
      swo_printf("Timer: 0x%" PRIX32 ", ", error_context->error_value);
      break;

    case MBED_ERROR_CODE_RTOS_MESSAGE_QUEUE_EVENT:
      swo_printf("MessageQueue: 0x%" PRIX32 ", ", error_context->error_value);
      break;

    case MBED_ERROR_CODE_ASSERTION_FAILED:
      swo_printf("Assertion failed: ");
      break;

    default:
      // Nothing to do here, just print the error info down
      break;
  }
  swo_printf("\nLocation: 0x%" PRIX32, error_context->error_address);
  swo_printf("\n-- MbedOS Error Info --\n");
}
#endif
