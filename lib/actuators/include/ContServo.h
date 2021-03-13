#pragma once

#include "Servo.h"

namespace Actuator {
class ContServo final : public Servo {
 public:
  typedef struct Config {
    PinName pwmPin;
    float max_speed;
    std::chrono::duration<float> max_pulse;
    std::chrono::duration<float> min_pulse;
    std::chrono::duration<float> period;
  } Config;
  ContServo(PinName pin, float max_speed, std::chrono::duration<float> max_pulse = DEFAULT_MAX,
            std::chrono::duration<float> min_pulse = DEFAULT_MIN, std::chrono::duration<float> period = DEFAULT_PERIOD);

  ContServo(const Config &config);

  /** Read the maximum speed of the servo
   *
   * @return The maximum speed of the servo
   */
  float getMaxSpeed(void) const;

  /** Set the speed of the servo
   *
   * @param speed The speed of the servo (can be negative)
   */
  void setValue(float speed) override;

 private:
  PwmOut m_pwm;
  float m_abs_max_speed;  // deg/sec
};
}  // namespace Actuator
