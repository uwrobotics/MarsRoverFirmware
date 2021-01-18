#pragma once

#include "Servo.h"

namespace ContServo {
typedef struct {
  PinName pwmPin;
  float max_speed;
  std::chrono::duration<float> max_pulse;
  std::chrono::duration<float> min_pulse;
  std::chrono::duration<float> period;
} Config;

class ContServo : public Servo {
 public:
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

  /** Read the current speed of the servo
   *
   * @return Current speed of servo
   */
  float getValue() const override;

 protected:
  PwmOut m_pwm;
  float m_abs_max_speed,  // deg/sec
      m_speed;            // deg/sec (from -m_abs_max_speed to +m_abs_max_speed)
};
}  // namespace ContServo
