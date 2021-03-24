#include "Logger.h"

#include "hal/itm_api.h"
#include "hal/serial_api.h"

extern serial_t stdio_uart;

namespace mbed {
// Redirect stdin, stdout, and stderr to Logger
FileHandle *mbed_target_override_console(int) {
  static Utility::Logger logger;
  return &logger;
}
}  // namespace mbed

namespace Utility {

Logger logger;
Mutex Logger::m_mutex;

Logger::Logger() {
  if constexpr (FeatureFlags::UART_LOGGING) {
    serial_init(&stdio_uart, STDIO_UART_TX, STDIO_UART_RX);
  }
}

ssize_t Logger::write(const void *buffer, size_t size) {
  std::scoped_lock<Mutex> lock(m_mutex);

  if constexpr (FeatureFlags::UART_LOGGING) {
    const char *c = static_cast<const char *>(buffer);
    for (size_t i = 0; i < size; i++) {
      serial_putc(&stdio_uart, *c++);
    }
  }

  if constexpr (FeatureFlags::SWO_LOGGING) {
    const char *c = static_cast<const char *>(buffer);
    for (size_t i = 0; i < size; i++) {
      ITM_SendChar(*c++);
    }
  }

  return size;
}

ssize_t Logger::read(void *buffer, size_t size) {
  // Reading is not supported by this file handle
  return -EBADF;
}

off_t Logger::seek(off_t offset, int whence) {
  // Seeking is not support by this file handler
  return -ESPIPE;
}

off_t Logger::size() {
  // Size is not defined for this file handle
  return -EINVAL;
}

int Logger::close() {
  return 0;
}

}  // namespace Utility
