#pragma once

#include "Servo.h"
#include "mbed.h"

class ContServo : public Servo {
 public:
  ContServo(PinName pin);
  ContServo(PinName pin, double max_speed);
  ContServo(PinName pin, double max_speed, std::chrono::duration<double> max_pulse,
            std::chrono::duration<double> min_pulse);

  mbed_error_status_t setMaxSpeed(double max_speed);
  double getMaxSpeed(void);

  // speed can be negative
  mbed_error_status_t setSpeed(double speed);
  double read(void);

 protected:
  double m_max_speed,  // MAXIMUM ROTATION SPEED in ANGLES PER SECOND (from -max speed to + max speed),  only valid for
                       // CONTINUOUS_SERVO types
      m_speed;         // ROTATING SPEED in ANGLES PER SECOND
};
