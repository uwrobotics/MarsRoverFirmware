#include "AEAT6012.h"

Encoder::AEAT6012::AEAT6012(PinName cs, PinName spi_mosi, PinName spi_clk)
    : m_position_deg(0), m_position_raw(0), m_cs(cs), m_spi(NC, spi_mosi, spi_clk) {
  m_spi.format(12, 2);
  m_spi.frequency(FREQUENCY_HZ);
  m_spi.set_dma_usage(DMA_USAGE_ALWAYS);

  m_cs = 1;  // Keep CS high until a read is requested
}

Encoder::AEAT6012::AEAT6012(const Config &config) : AEAT6012(config.cs, config.spi_mosi, config.spi_clk) {}

void Encoder::AEAT6012::read(void) {
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

  float curr_pos = raw_to_degrees(raw_data);

  // Get time delta
  m_timer.stop();
  float dt = std::chrono::duration_cast<std::chrono::nanoseconds>(m_timer.elapsed_time()).count();
  m_timer.reset();
  m_timer.start();

  // Avoid divide by zero
  if (dt > FLOAT_COMPARE_TOLERANCE) {
    // Update velocity
    m_angular_velocity_dps = (curr_pos - m_position_deg) / dt * 1000000000;
  }

  // Update posiiton data
  m_position_raw = raw_data;
  m_position_deg = curr_pos;
}

float Encoder::AEAT6012::read_position(void) {
  read();
  return get_position_deg();
}

float Encoder::AEAT6012::read_angular_velocity(void) {
  read();
  return get_angular_velocity_dps();
}

bool Encoder::AEAT6012::read_async(callback_ptr callback) {
  // Set up user callback
  m_callback = callback;

  // Assert CS to start signaling
  m_cs = 0;

  // Specced delay >500ns after CS assertion
  // ThisThread::sleep_for(1ms);

  // Write low dummy bytes to recieve
  int status =
      m_spi.transfer(dummy_buffer, 2, read_buffer, 2, event_callback_t(this, &Encoder::AEAT6012::priv_callback));

  // status = 0 => SPI transfer started
  // status = -1 => SPI peripheral is busy
  return (status == 0);
}

void Encoder::AEAT6012::priv_callback(int event) {
  // Deassert CS to stop signaling
  m_cs = 1;

  // Received data stream: -xxxx xxxx  xxxx x---
  // byte[0]: bits 6:0
  // byte[1]: bits 7:3

  uint16_t raw_data = ((read_buffer[0] & ~(static_cast<uint8_t>(1 << 7))) << 5) | (read_buffer[1] >> 3);

  float curr_pos = raw_to_degrees(raw_data);

  // Get time delta
  m_timer.stop();
  float dt = std::chrono::duration_cast<std::chrono::nanoseconds>(m_timer.elapsed_time()).count();
  m_timer.reset();
  m_timer.start();

  // Avoid divide by zero
  if (dt > FLOAT_COMPARE_TOLERANCE) {
    // Update velocity
    m_angular_velocity_dps = (curr_pos - m_position_deg) / dt * 1000000000;
  }

  // Update position data
  m_position_raw = raw_data;
  m_position_deg = curr_pos;

  // User callback
  m_callback();
}

float Encoder::AEAT6012::get_position_deg(void) {
  return m_position_deg;
}

uint16_t Encoder::AEAT6012::get_position_raw(void) {
  return m_position_raw;
}

float Encoder::AEAT6012::get_angular_velocity_dps(void) {
  return m_angular_velocity_dps;
}

float Encoder::AEAT6012::raw_to_degrees(uint16_t raw) {
  return raw / 4096.0 * 360.0;  // 12 bit resolution, 2^12 = 4096
}
