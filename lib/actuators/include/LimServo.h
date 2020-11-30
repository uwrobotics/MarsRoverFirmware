#pragma once

#include "Servo.h"

class LimServo : public Servo {
 public:
  LimServo(PinName pin);
  LimServo(PinName pin, float range);
  LimServo(PinName pin, float range, std::chrono::duration<float> max_pulse_ms,
           std::chrono::duration<float> min_pulse_ms);

  /** Set the range of the servo
   *
   * @param range The range of the servo in degrees
   */
  void setRange(float range);

  /** Read the range of the servo
   *
   * @return The range of the servo in degrees
   */
  float getRange(void) const;

  /** Set the position of the servo
   *
   * @param angle The position of the servo in degrees
   */
  void setValue(float position) override;
  LimServo& operator=(float position);

  /** Read the current position of the servo
   *
   * @return Current position of servo in degrees
   */
  float getValue() const override;

  // Override default period (ONLY USE FOR SPECIFIC FREQ REQUIREMENT)
  void setPeriod(std::chrono::duration<float> period) {
    m_pwm.period(period.count());
  };

 protected:
  static constexpr int DEFAULT_RANGE = 180;  // DEFAULT RANGE for LIMITED Servos

  float m_range,  // RANGE OF MOTION (from -range to + range), only valid for LIMIT_SERVO types
      m_pos;      // POSITION of servo, can be negative
  PinName m_pin;  // PIN the servo PWM signal is attached to
  PwmOut m_pwm;
};