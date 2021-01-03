#pragma once

class Servo;

namespace LimServo {
typedef struct {
  PinName pwmPin;
  float range;
  std::chrono::duration<float> max_pulse;
  std::chrono::duration<float> min_pulse;
  std::chrono::duration<float> period;
} Config;

class LimServo : public Servo {
 public:
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
  static constexpr int DEFAULT_RANGE = 180;

  float m_range,  // MAXIMUM ROTATION RANGE in degrees (from -range to + range)
      m_pos;      // POSITION of servo in degrees, can be negative
  PwmOut m_pwm;
};
}  // namespace LimServo