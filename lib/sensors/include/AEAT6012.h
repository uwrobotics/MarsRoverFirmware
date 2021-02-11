#pragma once

/*
 * Broadcom's AEAT-6012 is an absolute magnetic encoder with 12 bit resolution and SSI interface
 * Datasheet: https://docs.broadcom.com/doc/AV02-0188EN
 */

namespace AEAT6012 {
typedef void (*callback_ptr)(void);

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
  // Invokes user callback once read transaction is complete
  // Returns true if SPI read successfully started, false if SPI peripheral is busy
  bool read_position_async(void);

  // Returns stored absolute position in degrees (without invoking an encoder read)
  float get_position_deg(void);

  // Returns stored raw encoder reading (without invoking an encoder read)
  uint16_t get_position_raw(void);

 private:
  static constexpr uint32_t DEFAULT_FREQUENCY_HZ = 1000000;  // 1MHz (max frequency given by datasheet)
  const char dummy_buffer[2]                     = {0x00, 0x00};
  char read_buffer[2]                            = {0x00, 0x00};

  uint16_t m_position_raw;  // Raw encoder reading
  DigitalOut m_cs;
  SPI m_spi;

  // User callback function to be invoked when an encoder read is complete (only for async reads)
  callback_ptr m_callback;

  // Clean-up helper callback function for asynchronous read
  void priv_callback(int event);
};
}  // namespace AEAT6012
