#include "AEAT6012.h"

AEAT6012::AEAT6012(PinName cs, PinName spi_mosi, PinName spi_clk, int frequency_hz = DEFAULT_FREQUENCY_HZ)
    : m_cs(cs), m_spi(NC, spi_miso, spi_clk) {
  m_spi.format(12, 3);  // TODO: check clk phase
  m_spi.frequency(frequency_hz);

  m_cs = 1;  // Keep CS high until a read is requested
}

float AEAT6012::read_position(void) {
  // Assert CS to start signaling
  m_cs = 0;

  // Specced delay >500ns after CS assertion
  ThisThread::sleep_for(1);

  // Write low dummy bytes to recieve
  m_spi.write(dummy_buffer, 2, read_buffer, 2);

  // TODO: Next step to use async SPI

  // Deassert CS to stop signaling
  m_cs = 1;

  // MAX 7F(MSB) FC(LSB) , MIN 0004   <- TODO: check with Alex

  uint16_t raw_data = (((uint16_t)read_buffer[1]) >> 3) | (((uint16_t)read_buffer[0]) << 5);  // TODO: check with Alex

  // Specced delay >500ns after CS deassertion
  ThisThread::sleep_for(1);

  return raw_data_to_degrees(raw_data);
}

static float AEAT6012::raw_data_to_degrees(uint16_t raw_data) {
  return raw_data / 4096.0 * 360.0;  // 12 bit resolution, 2^12 = 4096  <- TODO: check this
}