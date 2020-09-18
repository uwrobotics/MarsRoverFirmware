#pragma once

#include "mbed.h"

class Servo {
 public:
 protected:
  PinName m_pin;  // PIN the servo PWM signal is attached to
  PwmOut m_pwm;

  static constexpr int PWM_FREQ     = 50;   // DEFAULT PWM FREQUENCY, should work for both length and positional control
  static constexpr auto DEFAULT_MAX = 2ms;  // DEFAULT MAX WAVE LENGTH
  static constexpr auto DEFAULT_MIN = 1ms;  // DEFAULT MIN WAVE LENGTH
  static constexpr std::chrono::duration<float> PERIOD = 1.0s / PWM_FREQ;  // DEFAULT PERIOD LENGTH

  std::chrono::duration<float> m_max_pulse,  // PULSE LENGTH for MAX ANGLE/SPEED
      m_min_pulse;                           // PULSE LENGTH for MIN ANGLE/SPEED

  int getSign(int val) {
    return (val >= 0) ? 1 : -1;
  }

 public:
  Servo(PinName pin) : m_pin(pin), m_pwm(pin){};
  virtual ~Servo(){};

  // Optionally implemented
  virtual mbed_error_status_t setRange(float range) {
    return MBED_ERROR_INVALID_OPERATION;
  };

  virtual mbed_error_status_t setMaxSpeed(float max_speed) {
    return MBED_ERROR_INVALID_OPERATION;
  };

  virtual float getRange(void) {
    return -1;
  };

  virtual float getMaxSpeed(void) {
    return -1;
  };

  virtual mbed_error_status_t setPosition(float angle) {
    return MBED_ERROR_INVALID_OPERATION;
  };

  virtual mbed_error_status_t setSpeed(float speed) {
    return MBED_ERROR_INVALID_OPERATION;
  };

  // Must be implemented
  virtual float read(void) = 0;

  // Override default period (ONLY USE FOR SPECIFIC FREQ REQUIREMENT)
  void setPeriod(std::chrono::duration<float> period) {
    m_pwm.period(period.count());
  };
};
