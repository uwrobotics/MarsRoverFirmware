#include "AEAT6012.h"

using namespace Encoder;

AEAT6012::AEAT6012(const Config &config)
    : m_position_deg(0),
      m_angular_velocity_deg_per_sec(0),
      m_position_raw(0),
      m_offset_deg(config.offset_deg),
      m_spi(NC, config.spi_mosi, config.spi_clk),
      m_cs(config.cs) {
  m_spi.format(12, 2);
  m_spi.frequency(FREQUENCY_HZ);
  m_spi.set_dma_usage(DMA_USAGE_ALWAYS);

  m_cs = 1;  // Keep CS high until a read is requested
}

bool AEAT6012::read(void) {
  m_cs = 0;  // Assert CS to start signaling

  // Specced delay >500ns after CS assertion
  // ThisThread::sleep_for(1ms);

  // Write low dummy bytes to recieve
  int num_bytes = m_spi.write(dummy_buffer, 2, read_buffer, 2);

  m_cs = 1;  // Deassert CS to stop signaling

  // On a successful encoder read, byte[0] bit 7 = 1, byte[1] bits 2:0 = 100
  if ((num_bytes == 2) && (read_buffer[0] >> 7) && ((read_buffer[1] & 0b111) == 0b100)) {
    // Specced delay >500ns after CS deassertion
    // ThisThread::sleep_for(1ms);

    // Received data stream: 1xxx xxxx  xxxx x100
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

    // Wrap-around catch for angular velocity estimation:
    //
    // If prev_pos = 359 deg and cur_pos = 1 deg, then the change in position would be 1 - 359 = -358
    //  => This is wrong, since the true change in position is 2
    //
    // To prevent this issue, we assume that the sampling rate is high enough and take the smaller of the following
    // two expressions to be the absolute change in position
    //   1. abs(cur_pos - prev_pos)
    //   2. 360 - abs(cur_pos - prev_pos)
    //
    // The direction depends on whether we are considering case 1 or case 2:
    //   1. Direction is positive if cur_pos > prev_pos, negative if cur_pos < prev_pos
    //   2. Direction is positive if cur_pos < prev_pos, negative if cur_pos > prev_pos

    // Avoid divide by zero
    if (dt > FLOAT_COMPARE_TOLERANCE) {
      // Determine change in position
      float delta_pos_abs = fabs(curr_pos - m_position_deg);
      int dir             = 1;

      float delta_pos_abs_wrapped = 360 - delta_pos_abs;

      // Choose the wrap-around change in position if it is the smaller one
      if (delta_pos_abs_wrapped < delta_pos_abs) {
        dir           = -1;
        delta_pos_abs = delta_pos_abs_wrapped;
      }

      dir = dir * (curr_pos > m_position_deg ? 1 : -1);

      // Update angular velocity (apply exponential moving average filter)
      float new_ang_vel              = dir * delta_pos_abs / dt * 1000000000;
      m_angular_velocity_deg_per_sec = MOVING_AVERAGE_FILTER_WEIGHT * new_ang_vel +
                                       (1 - MOVING_AVERAGE_FILTER_WEIGHT) * m_angular_velocity_deg_per_sec;
    }

    // Update posiiton data
    m_position_deg = curr_pos;
    m_position_raw = raw_data;

    return true;
  } else {
    return false;
  }
}

bool AEAT6012::getAngleDeg(float &angle) {
  std::scoped_lock<Mutex> lock(m_mutex);
  bool success = read();
  angle        = m_position_deg;
  return success;
}

bool AEAT6012::getAngularVelocityDegPerSec(float &speed) {
  std::scoped_lock<Mutex> lock(m_mutex);
  bool success = read();
  speed        = m_angular_velocity_deg_per_sec;
  return success;
}

bool AEAT6012::reset(void) {
  std::scoped_lock<Mutex> lock(m_mutex);

  bool success = read();
  m_offset_deg = rawToDegrees(m_position_raw);

  m_position_deg                 = 0;
  m_angular_velocity_deg_per_sec = 0;

  return success;
}

bool AEAT6012::readAsync(callback_ptr callback) {
  std::scoped_lock<Mutex> lock(m_mutex);

  // Set up user callback
  m_callback = callback;

  m_cs = 0;  // Assert CS to start signaling

  // Specced delay >500ns after CS assertion
  // ThisThread::sleep_for(1ms);

  // Write low dummy bytes to recieve
  int status = m_spi.transfer(dummy_buffer, 2, read_buffer, 2, event_callback_t(this, &AEAT6012::privCallback));

  // status = 0 => SPI transfer started
  // status = -1 => SPI peripheral is busy
  return (status == 0);
}

void AEAT6012::privCallback(int event) {
  m_cs = 1;  // Deassert CS to stop signaling

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

  // Wrap-around catch for angular velocity estimation:
  //
  // If prev_pos = 359 deg and cur_pos = 1 deg, then the change in position would be 1 - 359 = -358
  //  => This is wrong, since the true change in position is 2
  //
  // To prevent this issue, we assume that the sampling rate is high enough and take the smaller of the following
  // two expressions to be the absolute change in position
  //   1. abs(cur_pos - prev_pos)
  //   2. 360 - abs(cur_pos - prev_pos)
  //
  // The direction depends on whether we are considering case 1 or case 2:
  //   1. Direction is positive if cur_pos > prev_pos, negative if cur_pos < prev_pos
  //   2. Direction is positive if cur_pos < prev_pos, negative if cur_pos > prev_pos

  // Avoid divide by zero
  if (dt > FLOAT_COMPARE_TOLERANCE) {
    // Determine change in position
    float delta_pos_abs = fabs(curr_pos - m_position_deg);
    int dir             = 1;

    float delta_pos_abs_wrapped = 360 - delta_pos_abs;

    // Choose the wrap-around change in position if it is the smaller one
    if (delta_pos_abs_wrapped < delta_pos_abs) {
      dir           = -1;
      delta_pos_abs = delta_pos_abs_wrapped;
    }

    dir = dir * (curr_pos > m_position_deg ? 1 : -1);

    // Update angular velocity (apply exponential moving average filter)
    float new_ang_vel              = dir * delta_pos_abs / dt * 1000000000;
    m_angular_velocity_deg_per_sec = MOVING_AVERAGE_FILTER_WEIGHT * new_ang_vel +
                                     (1 - MOVING_AVERAGE_FILTER_WEIGHT) * m_angular_velocity_deg_per_sec;
  }

  // Update position data
  m_position_deg = curr_pos;
  m_position_raw = raw_data;

  // User callback
  m_callback();
}

float AEAT6012::getAngleDegNoTrigger(void) {
  return m_position_deg;
}

float AEAT6012::getAngularVelocityDegPerSecNoTrigger(void) {
  return m_angular_velocity_deg_per_sec;
}

float AEAT6012::rawToDegrees(uint16_t raw) {
  return raw / 4096.0 * 360.0;  // 12 bit resolution, 2^12 = 4096
}
