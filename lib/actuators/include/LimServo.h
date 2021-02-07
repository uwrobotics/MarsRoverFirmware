#pragma once

#include "Servo.h"

namespace Actuator {
class LimServo : public Servo {
 public:
  typedef struct Config {
    PinName pwmPin;
    float range;
    std::chrono::duration<float> max_pulse;
    std::chrono::duration<float> min_pulse;
    std::chrono::duration<float> period;
  } Config;
  LimServo(PinName pin, float range, std::chrono::duration<float> max_pulse = DEFAULT_MAX,
           std::chrono::duration<float> min_pulse = DEFAULT_MIN, std::chrono::duration<float> period = DEFAULT_PERIOD);

  LimServo(const Config &config);

  /** Read the range of the servo
   *
   * @return The range of the servo in degrees
   */
  float getRange(void) const;

  /** Set the position of the servo
   *
   * @param position The position of the servo in degrees
   */
  void setValue(float position) override;

  /** Read the current position of the servo
   *
   * @return Current position of servo in degrees
   */
  float getValue() const override;

 protected:
  static constexpr int DEFAULT_RANGE = 180;  // degrees

  PwmOut m_pwm;
  float m_abs_range,  // degrees
      m_pos;          // degrees (from -m_abs_range to +m_abs_range)
};
}  // namespace Actuator