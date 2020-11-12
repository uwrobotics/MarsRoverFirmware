#pragma once

#include "Servo.h"

class ContServo : public Servo {
 public:
  ContServo(PinName pin);
  ContServo(PinName pin, bool inverted, float max_speed);
  ContServo(PinName pin, bool inverted, float max_speed, std::chrono::duration<float> max_pulse,
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
  float getMaxSpeed(void);

  /** Set the speed of the servo
   *
   * @param speed The speed of the servo (can be negative)
   */
  void setValue(float speed);
  ContServo& operator=(float speed);

  /** Read the current speed of the servo
   *
   * @return Current speed of servo
   */
  float getValue();

 protected:
  float m_max_speed,  // MAXIMUM ROTATION SPEED in ANGLES PER SECOND (from -max_speed to + max_speed),  only valid for
                      // CONTINUOUS_SERVO types
      m_speed;        // ROTATING SPEED in ANGLES PER SECOND
  bool m_inverted;
};
