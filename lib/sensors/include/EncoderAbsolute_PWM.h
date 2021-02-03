#pragma once

#include "Encoder.h"
#include "PwmIn.h"
#include "mbed.h"

class EncoderAbsolute_PWM : public Encoder {
 public:
  EncoderAbsolute_PWM(t_encoderConfig config);
  ~EncoderAbsolute_PWM();

  // Must be implemented
  t_encoderType getType();
  float getAngle_Degrees();
  float getVelocity_DegreesPerSec();

  mbed_error_status_t reset();

 private:
  PwmIn m_pwmIn;

  float m_degreesPerUnit;
  float m_zeroOffset_Degrees;
};
