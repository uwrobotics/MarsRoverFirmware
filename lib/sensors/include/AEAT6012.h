#pragma once

/*
 * Broadcom's AEAT-6012 is an absolute magnetic encoder with 12 bit resolution and SPI interface
 * Datasheet: https://docs.broadcom.com/doc/AV02-0188EN
 *
 * TODO:
 * - Make driver async (need to configure DMAUsage?)
 * - Incorporate under Encoder base class
 */

namespace AEAT6012 {
typedef void (*callback_ptr)(int);

typedef struct {
  PinName cs;
  PinName spi_mosi;
  PinName spi_clk;
  uint32_t frequency_hz;
  callback_ptr callback;
} Config;

class AEAT6012 {
 public:
  AEAT6012(PinName cs, PinName spi_mosi, PinName spi_clk, callback_ptr callback,
           uint32_t frequency_hz = DEFAULT_FREQUENCY_HZ);
  AEAT6012(const Config &config);

  // Returns absolute position in degrees (blocking call)
  float read_position(void);

  // Asynchronous API for reading encoder position
  // Returns true if SPI read successfully started, false if SPI peripheral is busy
  bool read_position_async(void);

  // Returns stored absolute position in degrees (without invoking an encoder read)
  float get_position(void);

 private:
  // TODO: need to test this frequency
  static constexpr uint32_t DEFAULT_FREQUENCY_HZ = 1000000;  // Max frequency given by datasheet
  const char dummy_buffer[2]                     = {0x00, 0x00};
  char read_buffer[2]                            = {0x00, 0x00};

  float m_position_deg;  // Encoder position in degrees
  DigitalOut m_cs;
  SPI m_spi;
  callback_ptr
      m_callback;  // User callback function to be invoked when an encoder read is complete (only for async reads)

  // Clean-up helper callback function for asynchronous read
  void priv_callback(int event);

  // Converts encoder reading to degrees
  static float raw_data_to_degrees(uint16_t raw_data);
};
}  // namespace AEAT6012
