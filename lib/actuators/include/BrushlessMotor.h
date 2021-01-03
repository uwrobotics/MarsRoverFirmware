#pragma once

class Actuator;

class BrushlessMotor : public Actuator {
 public:
  BrushlessMotor(PinName pin, float max_speed, std::chrono::duration<float> max_pulse = DEFAULT_MAX,
                 std::chrono::duration<float> min_pulse = DEFAULT_MIN, std::chrono::duration<float> period = DEFAULT_PERIOD);

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
  static constexpr int PWM_FREQ     = 50;   // DEFAULT PWM FREQUENCY, should work for both length and positional control
  static constexpr auto DEFAULT_MAX = 2ms;  // DEFAULT MAX WAVE LENGTH
  static constexpr auto DEFAULT_MIN = 1ms;  // DEFAULT MIN WAVE LENGTH
  static constexpr std::chrono::duration<float> DEFAULT_PERIOD = 1.0s / PWM_FREQ;  // DEFAULT PERIOD LENGTH

  float m_max_speed,  // MAXIMUM ROTATIONAL SPEED (from -max_speed to + max_speed)
      m_speed;        // SPEED of motor, can be negative
  PwmOut m_pwm;
};
