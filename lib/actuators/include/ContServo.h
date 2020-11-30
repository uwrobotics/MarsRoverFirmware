#pragma once

#include "Servo.h"

class ContServo : public Servo {
 public:
  ContServo(PinName pin);
  ContServo(PinName pin, float max_speed);
  ContServo(PinName pin, float max_speed, std::chrono::duration<float> max_pulse,
            std::chrono::duration<float> min_pulse);

  /** Set the maximum speed of the servo
   *
   * @param max_speed The maximum speed of the servo
   */
  void setMaxSpeed(float max_speed);

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
  ContServo& operator=(float speed);

  /** Read the current speed of the servo
   *
   * @return Current speed of servo
   */
  float getValue() const override;

  // Override default period (ONLY USE FOR SPECIFIC FREQ REQUIREMENT)
  void setPeriod(std::chrono::duration<float> period) {
    m_pwm.period(period.count());
  };

 protected:
  float m_max_speed,  // MAXIMUM ROTATION SPEED in ANGLES PER SECOND (from -max_speed to + max_speed),  only valid for
                      // CONTINUOUS_SERVO types
      m_speed;        // ROTATING SPEED in ANGLES PER SECOND
  PinName m_pin;      // PIN the servo PWM signal is attached to
  PwmOut m_pwm;
};
