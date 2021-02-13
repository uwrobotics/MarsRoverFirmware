#pragma once

/*
 * Broadcom's AEAT-6012 is an absolute magnetic encoder with 12 bit resolution and SSI interface
 * Datasheet: https://docs.broadcom.com/doc/AV02-0188EN
 */

namespace Encoder {
typedef void (*callback_ptr)(void);

typedef struct {
  PinName cs;
  PinName spi_mosi;
  PinName spi_clk;
} Config;

class AEAT6012 {
 public:
  AEAT6012(PinName cs, PinName spi_mosi, PinName spi_clk);
  AEAT6012(const Config &config);

  // Trigger a blocking encoder read
  void read(void);

  // Trigger a blocking encoder read and return absolute position in degrees
  float read_position(void);

  // Trigger a blocking encoder read and return angular velocity in degrees / second
  float read_angular_velocity(void);

  // Asynchronous API for triggering encoder read
  // Invokes user callback once read transaction is complete
  // Returns true if SPI read successfully started, false if SPI peripheral is busy
  bool read_async(callback_ptr callback);

  // Returns stored absolute position in degrees (without invoking an encoder read)
  float get_position_deg(void);

  // Returns stored raw encoder reading (without invoking an encoder read)
  uint16_t get_position_raw(void);

  // Returns stored angular velocity in degrees / second (without invoking an encoder read)
  float get_angular_velocity_dps(void);

 private:
  static constexpr uint32_t FREQUENCY_HZ         = 1000000;  // 1MHz (max frequency given by datasheet)
  static constexpr float FLOAT_COMPARE_TOLERANCE = 1e-6;
  const char dummy_buffer[2]                     = {0x00, 0x00};
  char read_buffer[2]                            = {0x00, 0x00};

  float m_position_deg;          // Degrees
  uint16_t m_position_raw;       // Raw encoder reading
  float m_angular_velocity_dps;  // Degrees per second
  DigitalOut m_cs;
  SPI m_spi;
  Timer m_timer;  // For estimating encoder velocity

  // User callback function to be invoked when an encoder read is complete (only for async reads)
  callback_ptr m_callback;

  // Clean-up helper callback function for asynchronous read
  void priv_callback(int event);

  static float raw_to_degrees(uint16_t raw);  // Converts raw encoding reading to position in degrees
};
}  // namespace Encoder
