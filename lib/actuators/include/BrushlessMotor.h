#pragma once

#include "Actuator.h"

namespace Actuator {
class BrushlessMotor final : public Actuator {
 public:
  typedef struct Config {
    PinName pwmPin;
    float max_speed;
    std::chrono::duration<float> max_pulse;
    std::chrono::duration<float> min_pulse;
    std::chrono::duration<float> period;
  } Config;
  BrushlessMotor(PinName pin, float max_speed, std::chrono::duration<float> max_pulse = DEFAULT_MAX,
                 std::chrono::duration<float> min_pulse = DEFAULT_MIN,
                 std::chrono::duration<float> period    = DEFAULT_PERIOD);

  BrushlessMotor(const Config &config);

  /** Read the maximum speed of the motor
   *
   * @return The maximum speed of the motor
   */
  float getMaxSpeed(void) const;

  /** Set the speed of the motor
   *
   * @param speed The speed of the motor (can be negative)
   */
  void setValue(float speed) override;

  /** Read the current speed of the motor
   *
   * @return Current speed of motor
   */
  float getValue() const override;

  // Override default period
  void setPeriod(std::chrono::duration<float> period);

 protected:
  // TODO: check these default values
  static constexpr int PWM_FREQ                                = 50;               // DEFAULT PWM FREQUENCY
  static constexpr auto DEFAULT_MAX                            = 2ms;              // DEFAULT MAX WAVE LENGTH
  static constexpr auto DEFAULT_MIN                            = 1ms;              // DEFAULT MIN WAVE LENGTH
  static constexpr std::chrono::duration<float> DEFAULT_PERIOD = 1.0s / PWM_FREQ;  // DEFAULT PERIOD LENGTH

  PwmOut m_pwm;
  float m_abs_max_speed,  // deg/sec
      m_speed;            // deg/sec (from -m_abs_max_speed to +m_abs_max_speed)
};
}  // namespace Actuator
