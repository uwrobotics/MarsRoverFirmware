#include "AEAT6012.h"

AEAT6012::AEAT6012::AEAT6012(PinName cs, PinName spi_mosi, PinName spi_clk, callback_ptr callback,
                             uint32_t frequency_hz)
    : m_position_raw(0), m_cs(cs), m_spi(NC, spi_mosi, spi_clk), m_callback(callback) {
  m_spi.format(12, 2);
  m_spi.frequency(frequency_hz);
  m_spi.set_dma_usage(DMA_USAGE_ALWAYS);

  m_cs = 1;  // Keep CS high until a read is requested
}

AEAT6012::AEAT6012::AEAT6012(const Config &config)
    : AEAT6012(config.cs, config.spi_mosi, config.spi_clk, config.callback, config.frequency_hz) {}

float AEAT6012::AEAT6012::read_position(void) {
  // Assert CS to start signaling
  m_cs = 0;

  // Specced delay >500ns after CS assertion
  // ThisThread::sleep_for(1ms);

  // Write low dummy bytes to recieve
  m_spi.write(dummy_buffer, 2, read_buffer, 2);

  // Deassert CS to stop signaling
  m_cs = 1;

  // Specced delay >500ns after CS deassertion
  // ThisThread::sleep_for(1ms);

  // Received data stream: -xxxx xxxx  xxxx x---
  // byte[0]: bits 6:0
  // byte[1]: bits 7:3

  uint16_t raw_data = ((read_buffer[0] & ~(static_cast<uint8_t>(1 << 7))) << 5) | (read_buffer[1] >> 3);

  // Update posiiton data
  m_position_raw = raw_data;

  return get_position_deg();
}

bool AEAT6012::AEAT6012::read_position_async(void) {
  // Assert CS to start signaling
  m_cs = 0;

  // Specced delay >500ns after CS assertion
  // ThisThread::sleep_for(1ms);

  // Write low dummy bytes to recieve
  int status =
      m_spi.transfer(dummy_buffer, 2, read_buffer, 2, event_callback_t(this, &AEAT6012::AEAT6012::priv_callback));

  // status = 0 => SPI transfer started
  // status = -1 => SPI peripheral is busy
  return (status == 0);
}

void AEAT6012::AEAT6012::priv_callback(int event) {
  // Deassert CS to stop signaling
  m_cs = 1;

  // Received data stream: -xxxx xxxx  xxxx x---
  // byte[0]: bits 6:0
  // byte[1]: bits 7:3

  uint16_t raw_data = ((read_buffer[0] & ~(static_cast<uint8_t>(1 << 7))) << 5) | (read_buffer[1] >> 3);

  // Update position data
  m_position_raw = raw_data;

  // User callback
  m_callback();
}

float AEAT6012::AEAT6012::get_position_deg(void) {
  // Convert raw encoder reading to degrees
  return m_position_raw / 4096.0 * 360.0;  // 12 bit resolution, 2^12 = 4096
}

uint16_t AEAT6012::AEAT6012::get_position_raw(void) {
  return m_position_raw;
}
