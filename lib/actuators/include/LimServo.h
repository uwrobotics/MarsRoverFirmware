#pragma once

class Servo;

class LimServo : public Servo {
 public:
  LimServo(PinName pin, float range, std::chrono::duration<float> max_pulse_ms = DEFAULT_MAX,
           std::chrono::duration<float> min_pulse_ms = DEFAULT_MIN, std::chrono::duration<float> period = DEFAULT_PERIOD);

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

  // Override default period (ONLY USE FOR SPECIFIC FREQ REQUIREMENT)
  void setPeriod(std::chrono::duration<float> period);

 protected:
  static constexpr int DEFAULT_RANGE = 180;

  float m_range,  // MAXIMUM ROTATION RANGE (from -range to + range)
      m_pos;      // POSITION of servo, can be negative
  PwmOut m_pwm;
};