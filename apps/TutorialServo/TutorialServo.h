#ifndef TUTORIAL_SERVO_H
#define TUTORIAL_SERVO_H

#include "mbed.h"


class TutorialServo
{
  public:
  TutorialServo(PinName servoPin, float servoFrequency, float degrees, float minPulseWidthFloat = 1, float maxPulseWidthFloat = 2);
  
  void setPositionInDegrees(float angleRange);
  void setPulseWidthRangeInMs(float minPulsewidthMsFloat, float maxPulsewidthMsFloat);
  

  private:
  PwmOut servoPwm;
  float minPulseWidthMs, maxPulseWidthMs, angleRange; //angleRange anywhere from plus/minus 90.
  void setAngleRangeInDegrees(float degrees);
};

#endif