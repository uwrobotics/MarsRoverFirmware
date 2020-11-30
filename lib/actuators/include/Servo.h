#pragma once

#include "Actuator.h"

class Servo : public Actuator {
 protected:
  static constexpr int PWM_FREQ     = 50;   // DEFAULT PWM FREQUENCY, should work for both length and positional control
  static constexpr auto DEFAULT_MAX = 2ms;  // DEFAULT MAX WAVE LENGTH
  static constexpr auto DEFAULT_MIN = 1ms;  // DEFAULT MIN WAVE LENGTH
  static constexpr std::chrono::duration<float> PERIOD = 1.0s / PWM_FREQ;  // DEFAULT PERIOD LENGTH

  std::chrono::duration<float> m_max_pulse,  // PULSE LENGTH for MAX ANGLE/SPEED
      m_min_pulse;                           // PULSE LENGTH for MIN ANGLE/SPEED

  static int getSign(int val) {
    return (val >= 0) ? 1 : -1;
  }
};
