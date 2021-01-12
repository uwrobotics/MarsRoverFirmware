#pragma once

#ifndef PWM_IN_DEFAULT_NUM_SAMPLES_TO_AVERAGE
#define PWM_IN_DEFAULT_NUM_SAMPLES_TO_AVERAGE 12
#endif

#include "Encoder.h"
#include "PwmIn.h"

namespace EncoderAbsolute_PWM {
typedef struct Config {
  PinName pwm;
  float degreesPerUnit, zeroOffsetDegrees;
} Config;
class EncoderAbsolute_PWM : public Encoder {
 public:
  EncoderAbsolute_PWM(const Config &config);
  ~EncoderAbsolute_PWM();

  bool getAngle_Degrees(float &theta) override;
  bool getVelocity_DegreesPerSec(float &thetaDot) override;

  bool reset() override;

 private:
  PwmIn m_pwmIn;
  float m_degreesPerUnit;
  float m_zeroOffsetDegrees;
};
}  // namespace EncoderAbsolute_PWM
