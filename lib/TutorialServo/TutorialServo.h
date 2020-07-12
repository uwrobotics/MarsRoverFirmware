
#ifndef TUTORIAL_SERVO_H
#define TUTORIAL_SERVO_H

#include "mbed.h"

class TutorialServo {
 private:
  PwmOut m_servoPwmOut;
  float m_servoRangeInDegrees;
  float m_minPulsewidthInMs;
  float m_maxPulsewidthInMs;

 public:
  TutorialServo(PinName servoPin, float servoRangeInDegrees = 180.0, float minPulsewidthInMs = 1.0,
                float maxPulsewidthInMs = 2.0);
  ~TutorialServo();
  void setAngleRangeInDegrees(float degrees);
  void setPulsewidthRangeInMs(float minPulsewidthMs, float maxPulsewidthMs);
  void setPositionInDegrees(float degrees);
};

#endif  // !TUTORIAL_SERVO_H
