#ifndef TUTORIAL_SERVO_H
#define TUTORIAL_SERVO_H

#include "mbed.h"

class TutorialServo {
 private:
  PmwOut m_servoPmwOut;
  float m_servoRangeInDegrees;
  float m_minPulsewidthInMs;
  float m_maxPulsewidthInMs;

 public:
  TutorialServo(PinName servoPin, float servoRangeInDegrees = 180.0, float minPulsewidthInMs = 1,
                float maxPulsewidthInMs = 2);
  ~TutorialServo();
  void setAngleRangeInDegrees(float degrees);
  void setPulsewidthRangeInMs(float minPulsewidthMs, float maxPulsewidthMs);
  void setPositionInDegrees(float degrees);
};

TutorialServo::TutorialServo(PinName servoPin, float servoRangeInDegrees = 180.0, float minPulsewidthInMs = 1,
                             float maxPulsewidthInMs = 2) {
  m_servoPmwOut         = servoPin;
  m_servoRangeInDegrees = servoRangeInDegrees;
  m_minPulsewidthInMs   = minPulsewidthInMs;
  m_maxPulsewidthInMs   = maxPulsewidthInMs;
}

TutorialServo::~TutorialServo() {
  m_servoRangeInDegrees = 0;
  m_minPulsewidthInMs   = 0;
  m_maxPulsewidthInMs   = 0;
}

#endif  // !TUTORIAL_SERVO_H
