#pragma once

#include "Servo.h"
#include "mbed.h"

class LimServo : public Servo {
 public:
  LimServo(PinName pin);
  LimServo(PinName pin, double range);
  LimServo(PinName pin, double range, std::chrono::duration<double> max_pulse_ms, std::chrono::duration<double> min_pulse_ms);

  mbed_error_status_t setRange(double range);
  double getRange(void);

  // Angle can be negative
  mbed_error_status_t setPosition(double angle);
  double read(void);

 protected:
  const int DEFAULT_RANGE = 180;  // DEFAULT RANGE for LIMITED Servos

  double m_range,  // RANGE OF MOTION (from -range to + range), only valid for LIMIT_SERVO types
      m_pos;      // POSITION of servo, can be negative
};
