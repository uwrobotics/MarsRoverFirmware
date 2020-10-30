#pragma once

#include "Servo.h"
#include "mbed.h"

class LimServo : public Servo {
 public:
  LimServo(PinName pin);
  LimServo(PinName pin, float range);
<<<<<<< HEAD
  LimServo(PinName pin, float range, std::chrono::duration<float> max_pulse, std::chrono::duration<float> min_pulse);
=======
  LimServo(PinName pin, float range, std::chrono::duration<float> max_pulse_ms,
           std::chrono::duration<float> min_pulse_ms);
>>>>>>> origin/master

  mbed_error_status_t setRange(float range);
  float getRange(void);

  // Angle can be negative
  mbed_error_status_t setPosition(float angle);
  float read(void);

 protected:
  const int DEFAULT_RANGE = 180;  // DEFAULT RANGE for LIMITED Servos

  float m_range,  // RANGE OF MOTION (from -range to + range), only valid for LIMIT_SERVO types
      m_pos;      // POSITION of servo, can be negative
};
