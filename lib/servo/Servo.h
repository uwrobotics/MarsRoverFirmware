#pragma once

#include "PwmOut.h"
#include "mbed.h"

class Servo {
 public:
 protected:
  PinName m_pin;  // PIN the servo PWM signal is attached to
  PwmOut m_pwm;   // PWM object, does not have default constructor so have to use INITIALIZER LIST to avoid COMPILER
                  // attempting to DEFAULT CONSTRUCT

  const int PWM_FREQ = 50,               // DEFAULT PWM FREQUENCY, should work for both length and positional control
      DEFAULT_MAX    = 2,                // DEFAULT MAX WAVE LENGTH in MILISECONDS
      DEFAULT_MIN    = 1,                // DEFAULT MIN WAVE LENGTH in MILISECONDS
      PERIOD         = 1000 / PWM_FREQ;  // DEFAULT PERIOD LENGTH in MILISECONDS

  float m_max_pulse_ms,  // PULSE LENGTH for MAX ANGLE
      m_min_pulse_ms;    // PULSE LENGTH for MIN ANGLE

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
  void setPeriod(int period) {
    m_pwm.period_us(period * 1000);
  };
};