#pragma once

#include <mutex>

#include "Encoder.h"
#include "mbed.h"

/*
 * Broadcom's AEAT-6012 is an absolute magnetic encoder with 12 bit resolution and SSI interface
 * Datasheet: https://docs.broadcom.com/doc/AV02-0188EN
 */

namespace Encoder {

class AEAT6012 final : public Encoder {
 public:
  typedef void (*callback_ptr)(void);

  typedef struct {
    PinName spi_clk;
    PinName spi_miso;
    PinName cs;
    float offset_deg;
  } Config;

  AEAT6012(const Config &config);

  // Trigger a blocking encoder read and store a value for angle and velocity
  // returns whether the read was successful
  [[nodiscard]] bool update() override;

  // Returns the stored angle
  float getAngleDeg() override;

  // Returns the stored speed
  float getAngularVelocityDegPerSec() override;

  // Reset encoder values and offset
  [[nodiscard]] bool reset(void) override;

  // Asynchronous API for triggering encoder read
  // Invokes user callback once read transaction is complete
  // Returns true if SPI read successfully started, false if SPI peripheral is busy
  bool update(callback_ptr callback);

 private:
  static constexpr uint32_t FREQUENCY_HZ              = 1000000;  // 1MHz (max frequency given by datasheet)
  static constexpr float FLOAT_COMPARE_TOLERANCE      = 1e-6;
  static constexpr float MOVING_AVERAGE_FILTER_WEIGHT = 0.8;

  const char dummy_buffer[2] = {0x00, 0x00};
  char read_buffer[2]        = {0x00, 0x00};

  float m_position_deg;
  float m_angular_velocity_deg_per_sec;
  uint16_t m_position_raw;
  float m_offset_deg;

  SPI m_spi;
  DigitalOut m_cs;
  Timer m_timer;
  Mutex m_mutex;

  // User callback function to be invoked when an encoder read is complete (only for async reads)
  callback_ptr m_callback;

  // Clean-up helper callback function for asynchronous read
  void privCallback(int event);

  // Trigger a blocking encoder read
  bool read(void);

  // Converts raw encoder reading to position in degrees
  static float rawToDegrees(uint16_t raw);
};
}  // namespace Encoder
