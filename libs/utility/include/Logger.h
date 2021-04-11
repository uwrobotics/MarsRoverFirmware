#pragma once

#include <mutex>
#include <sstream>

#include "mbed.h"
#include "uwrt_config_target.h"

namespace Utility {

class Logger;
extern Logger logger;

class Logger : public FileHandle {
 public:
  Logger();

  template <typename T>
  Logger& operator<<(T out) {
    std::scoped_lock<Mutex> lock(m_mutex);

    std::stringstream ss;
    ss << out;

    write(static_cast<const char*>(ss.str().c_str()), ss.str().length());

    return *this;
  }

  // FileHandle overrides
  ssize_t read(void* buffer, size_t size) override;
  ssize_t write(const void* buffer, size_t size) override;
  off_t seek(off_t offset, int whence = SEEK_SET) override;
  int close() override;
  off_t size() override;

 private:
  static Mutex m_mutex;
};

}  // namespace Utility
