#pragma once

#include <mutex>

/*
 * Broadcom's AEAT-6012 is an absolute magnetic encoder with 12 bit resolution and SSI interface
 * Datasheet: https://docs.broadcom.com/doc/AV02-0188EN
 */

namespace Encoder {
typedef void (*callback_ptr)(void);

typedef struct {
  PinName spi_mosi;
  PinName spi_clk;
  PinName cs;
  float offset_deg;
} Config;

class AEAT6012 {
 public:
  AEAT6012(PinName spi_mosi, PinName spi_clk, PinName cs = NC, float offset_deg = 0.0f);
  AEAT6012(const Config &config);

  // Trigger a blocking encoder read and retrieve the absolute position in degrees
  // Returns whether the read was successful
  bool getAngleDeg(float &angle);

  // Trigger a blocking encoder read and retrieve the angular velocity in degrees / second
  // Returns whether the read was successful
  bool getAngularVelocityDegPerSec(float &speed);

  // Reset encoder values and offset
  bool reset(void);

  // Asynchronous API for triggering encoder read
  // Invokes user callback once read transaction is complete
  // Returns true if SPI read successfully started, false if SPI peripheral is busy
  bool readAsync(callback_ptr callback);

  // For async use - returns stored absolute position in degrees (without invoking an encoder read)
  float getAngleDegNoTrigger(void);

  // For async use - returns stored angular velocity in degrees / second (without invoking an encoder read)
  float getAngularVelocityDegPerSecNoTrigger(void);

 private:
  static constexpr uint32_t FREQUENCY_HZ              = 1000000;  // 1MHz (max frequency given by datasheet)
  static constexpr float FLOAT_COMPARE_TOLERANCE      = 1e-6;
  static constexpr float MOVING_AVERAGE_FILTER_WEIGHT = 0.7;

  const char dummy_buffer[2] = {0x00, 0x00};
  char read_buffer[2]        = {0x00, 0x00};

  float m_position_deg;          // Degrees
  float m_angular_velocity_dps;  // Degrees per second
  float m_offset_deg;            // Offset in degrees
  uint16_t m_position_raw;       // Raw encoder reading

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

  // Converts raw encoding reading to position in degrees
  static float rawToDegrees(uint16_t raw);
};
}  // namespace Encoder
