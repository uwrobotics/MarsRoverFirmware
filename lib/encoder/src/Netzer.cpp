#include "Netzer.h"

using namespace Encoder;

Netzer::Netzer(PinName mosi, PinName miso, PinName sclk, callback_ptr callback, float offset_deg)
    : m_spi(mosi, miso, sclk),
      m_callback(callback),
      m_offset_deg(offset_deg),
      m_position_deg(0),
      m_angular_velocity_deg_per_sec(0),
      m_position_raw(0) {
  m_spi.format(16, 3);
  m_spi.frequency(DEFAULT_FREQUENCY_HZ);
  m_spi.set_dma_usage(DMA_USAGE_ALWAYS);
}

bool Netzer::read(void) {
  int num_bytes = m_spi.write(tx_buffer, WORDS, rx_buffer, WORDS);
  // TODO: Add check for error bit?
  if ((num_bytes == WORDS)) {
    angularVelocityEstimation();
    return true;
  } else {
    return false;
  }
}

bool Netzer::readAsync(callback_ptr callback) {
  std::scoped_lock<Mutex> lock(m_mutex);
  m_callback = callback;
  // return status
  int status = m_spi.transfer(tx_buffer, WORDS, rx_buffer, WORDS, event_callback_t(this, &Netzer::priv_spi_callback));
  // status = 0 => SPI transfer started
  // status = -1 => SPI peripheral is busy
  return (status == 0);
}

uint16_t Netzer::get_raw_data() {
  return m_raw_data;
}

void Netzer::priv_spi_callback(int events) {
  angularVelocityEstimation();
  m_callback();
}

bool Netzer::reset(void) {
  std::scoped_lock<Mutex> lock(m_mutex);

  bool success = read();
  m_offset_deg = rawAbsToDegrees(m_position_raw);

  m_position_deg                 = 0;
  m_angular_velocity_deg_per_sec = 0;

  return success;
}

bool Netzer::getAngleDeg(float &angle) {
  std::scoped_lock<Mutex> lock(m_mutex);
  bool success = read();
  angle        = m_position_deg;
  return success;
}

bool Netzer::getAngularVelocityDegPerSec(float &speed) {
  std::scoped_lock<Mutex> lock(m_mutex);
  bool success = read();
  speed        = m_angular_velocity_deg_per_sec;
  return success;
}

float Netzer::getAngleDegAsync(void) {
  return m_position_deg;
}

float Netzer::getAngularVelocityDegPerSecAsync(void) {
  return m_angular_velocity_deg_per_sec;
}

float Netzer::rawAbsToDegrees(uint16_t raw) {
  return raw / 65536.0 * 360.0;  // 16 bit resolution, 2^16 = 65536
}

// Wrapper function  (taken from AEAT6012 estimator, maybe we can
// refactor both to use a general estimator function like this?)
void Netzer::angularVelocityEstimation(void) {
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

  // Not sure if I'm handling the payload correctly
  uint32_t raw_data = ((static_cast<uint32_t>(rx_buffer[0]) << 24) | (static_cast<uint32_t>(rx_buffer[1]) << 16)) |
                      ((static_cast<uint32_t>(rx_buffer[0]) << 8) | (static_cast<uint32_t>(rx_buffer[0])));

  m_raw_data = (uint16_t)((raw_data << 8) >> 8);

  float curr_pos = rawAbsToDegrees(m_raw_data) - m_offset_deg;
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
}