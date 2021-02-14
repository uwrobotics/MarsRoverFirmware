#include "AEAT6012.h"

Encoder::AEAT6012::AEAT6012(PinName cs, PinName spi_mosi, PinName spi_clk, float offset_deg)
    : m_position_deg(0), m_position_raw(0), m_offset_deg(offset_deg), m_cs(cs), m_spi(NC, spi_mosi, spi_clk) {
  m_spi.format(12, 2);
  m_spi.frequency(FREQUENCY_HZ);
  m_spi.set_dma_usage(DMA_USAGE_ALWAYS);

  m_cs = 1;  // Keep CS high until a read is requested
}

Encoder::AEAT6012::AEAT6012(const Config &config)
    : AEAT6012(config.cs, config.spi_mosi, config.spi_clk, config.offset_deg) {}

bool Encoder::AEAT6012::read(void) {
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

  float curr_pos = rawToDegrees(raw_data) - m_offset_deg;
  if (curr_pos < 0) {
    curr_pos += 360;
  }

  // Get time delta
  m_timer.stop();
  float dt = std::chrono::duration_cast<std::chrono::nanoseconds>(m_timer.elapsed_time()).count();
  m_timer.reset();
  m_timer.start();

  // Avoid divide by zero
  if (dt > FLOAT_COMPARE_TOLERANCE) {
    // Determine change in position
    float delta_pos_abs = fabs(curr_pos - m_position_deg);
    float dir           = 1;

    float delta_pos_abs_wrapped = 360 - delta_pos_abs;

    // Choose the wrap-around change in position if it is the smaller one
    if (delta_pos_abs_wrapped < delta_pos_abs) {
      dir           = -1;
      delta_pos_abs = delta_pos_abs_wrapped;
    }

    dir = dir * (curr_pos > m_position_deg ? 1 : -1);

    // Update velocity (apply moving average low pass filter)
    float new_ang_vel = dir * delta_pos_abs / dt * 1000000000;
    m_angular_velocity_dps =
        MOVING_AVERAGE_FILTER_WEIGHT * new_ang_vel + (1 - MOVING_AVERAGE_FILTER_WEIGHT) * m_angular_velocity_dps;
  }

  // Update posiiton data
  m_position_raw = raw_data;
  m_position_deg = curr_pos;

  return true;
}

bool Encoder::AEAT6012::getAngleDeg(float &angle) {
  std::scoped_lock<Mutex> lock(m_mutex);
  bool success = read();
  angle        = getAngleDegNoTrigger();
  return success;
}

bool Encoder::AEAT6012::getAngularVelocityDegPerSec(float &speed) {
  std::scoped_lock<Mutex> lock(m_mutex);
  bool success = read();
  speed        = getAngularVelocityDegPerSecNoTrigger();
  return success;
}

bool Encoder::AEAT6012::readAsync(callback_ptr callback) {
  std::scoped_lock<Mutex> lock(m_mutex);

  // Set up user callback
  m_callback = callback;

  // Assert CS to start signaling
  m_cs = 0;

  // Specced delay >500ns after CS assertion
  // ThisThread::sleep_for(1ms);

  // Write low dummy bytes to recieve
  int status =
      m_spi.transfer(dummy_buffer, 2, read_buffer, 2, event_callback_t(this, &Encoder::AEAT6012::privCallback));

  // status = 0 => SPI transfer started
  // status = -1 => SPI peripheral is busy
  return (status == 0);
}

void Encoder::AEAT6012::privCallback(int event) {
  // Deassert CS to stop signaling
  m_cs = 1;

  // Received data stream: -xxxx xxxx  xxxx x---
  // byte[0]: bits 6:0
  // byte[1]: bits 7:3

  uint16_t raw_data = ((read_buffer[0] & ~(static_cast<uint8_t>(1 << 7))) << 5) | (read_buffer[1] >> 3);

  float curr_pos = rawToDegrees(raw_data) - m_offset_deg;
  if (curr_pos < 0) {
    curr_pos += 360;
  }

  // Get time delta
  m_timer.stop();
  float dt = std::chrono::duration_cast<std::chrono::nanoseconds>(m_timer.elapsed_time()).count();
  m_timer.reset();
  m_timer.start();

  // Avoid divide by zero
  if (dt > FLOAT_COMPARE_TOLERANCE) {
    // Determine change in position
    float delta_pos_abs = fabs(curr_pos - m_position_deg);
    float dir           = 1;

    float delta_pos_abs_wrapped = 360 - delta_pos_abs;

    // Choose the wrap-around change in position if it is the smaller one
    if (delta_pos_abs_wrapped < delta_pos_abs) {
      dir           = -1;
      delta_pos_abs = delta_pos_abs_wrapped;
    }

    dir = dir * (curr_pos > m_position_deg ? 1 : -1);

    // Update velocity (apply moving average low pass filter)
    float new_ang_vel = dir * delta_pos_abs / dt * 1000000000;
    m_angular_velocity_dps =
        MOVING_AVERAGE_FILTER_WEIGHT * new_ang_vel + (1 - MOVING_AVERAGE_FILTER_WEIGHT) * m_angular_velocity_dps;
  }

  // Update position data
  m_position_raw = raw_data;
  m_position_deg = curr_pos;

  // User callback
  m_callback();
}

bool Encoder::AEAT6012::reset(void) {
  std::scoped_lock<Mutex> lock(m_mutex);

  bool success = getAngleDeg(m_offset_deg);

  m_position_deg         = 0;
  m_position_raw         = 0;
  m_angular_velocity_dps = 0;

  return success;
}

float Encoder::AEAT6012::getAngleDegNoTrigger(void) {
  return m_position_deg;
}

uint16_t Encoder::AEAT6012::getPositionRawNoTrigger(void) {
  return m_position_raw;
}

float Encoder::AEAT6012::getAngularVelocityDegPerSecNoTrigger(void) {
  return m_angular_velocity_dps;
}

float Encoder::AEAT6012::rawToDegrees(uint16_t raw) {
  return raw / 4096.0 * 360.0;  // 12 bit resolution, 2^12 = 4096
}
