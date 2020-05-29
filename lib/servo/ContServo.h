#pragma once

#include "Servo.h"
#include "mbed.h"

class ContServo : public Servo {
 public:
  ContServo(PinName pin);
  ContServo(PinName pin, float max_speed);
  ContServo(PinName pin, float max_speed, float max_pulse_ms, float min_pulse_ms);

  mbed_error_status_t setMaxSpeed(float max_speed);
  float getMaxSpeed(void);

  // speed can be negative
  mbed_error_status_t setSpeed(float speed);
  float read(void);

 protected:
  float m_max_speed,  // MAXIMUM ROTATION SPEED in ANGLES PER SECOND (from -max speed to + max speed),  only valid for
                      // CONTINUOUS_SERVO types
      m_speed;        // ROTATING SPEED in ANGLES PER SECOND
};