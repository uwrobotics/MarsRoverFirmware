#pragma once

#ifndef PWM_IN_DEFAULT_NUM_SAMPLES_TO_AVERAGE
#define PWM_IN_DEFAULT_NUM_SAMPLES_TO_AVERAGE 12
#endif

#include "Encoder.h"
#include "PwmIn.h"

namespace Encoder{
typedef struct Config {
  PinName pwmIn;
  float degreesPerUnit, zeroOffsetDegrees;
} Config;
class Absolute_PWM : public Encoder {
 public:
  Absolute_PWM(PwmIn pwmIn, float degreesPerUnit, float zeroOffsetDegrees);

  Absolute_PWM(const Config &config);
  ~Absolute_PWM();

  bool getAngle_Degrees(float &theta) override;
  bool getVelocity_DegreesPerSec(float &thetaDot) override;

  bool reset() override;

 private:
  PwmIn m_pwmIn;
  float m_degreesPerUnit;
  float m_zeroOffsetDegrees;
};
}  // namespace Encoder
