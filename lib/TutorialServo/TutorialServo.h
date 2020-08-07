
#pragma once
// this header file is for my custom servo class
#include "mbed.h"

class TutorialServo {  // this class is based on the template provided in the training document
 private:
  PwmOut m_servoPwmOut;
  float m_servoRangeInDegrees;
  float m_minPulsewidthInMs;
  float m_maxPulsewidthInMs;

 public:
  TutorialServo(PinName servoPin, float servoRangeInDegrees = 180.0, float minPulsewidthInMs = 1.0,
                float maxPulsewidthInMs = 2.0);
  // setAngleRange will take a positive range of motion
  // angle range will represent the range of motion between min pulse and max pulse
  void setAngleRangeInDegrees(float degrees);
  void setPulsewidthRangeInMs(float minPulsewidthMs, float maxPulsewidthMs);
  // setPosition will take a positive angle
  // angle 0 will represent the min pulse width
  // range / 2 will represent the default position of servo
  void setPositionInDegrees(float degrees);
  float getAngleRangeInDegrees();
};
