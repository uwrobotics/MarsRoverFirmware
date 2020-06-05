#ifndef TUTORIAL_SERVO_H
#define TUTORIAL_SERVO_H

#include "mbed.h"


class TutorialServo
{
  public:
  TutorialServo(PinName servoPin, float servoFrequency);
  void setAngleRangeInDegrees(float degrees);
  void setPulseWidthRangeInMs(float minPulsewidthMsFloat, float maxPulsewidthMsFloat);
  void setPositionInDegrees(float degrees);

  private:
  PwmOut servoPwm;
  float minPulseWidthMs, maxPulseWidthMs, angleRange; //angleRange anywhere from plus/minus 90.
};

#endif