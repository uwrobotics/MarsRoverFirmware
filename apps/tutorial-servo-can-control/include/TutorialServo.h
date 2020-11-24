#pragma once

#include "mbed.h"

class TutorialServo {
 public:
  // Constructor
  // Takes in servo pin name, and optionally a servo range (default value of
  // 180.0 degrees), a min pulsewidth (default 1ms) and max pulsewidth (default
  // 2ms)

  TutorialServo(PinName servoPin, float servoRangeInDegrees = 180.0, float minPulsewidthInMs = 1,
                float maxPulsewidthInMs = 2);

  // Set servo position in degrees (ex. 45deg)
  void setPositionInDegrees(const float degrees);

  // Set servo position in percentage of range of motion (ex. 50%)
  void setPositionInPercentage(const float percentage);

  // Get the servo range in degrees (ex. 90deg)
  float getServoRangeInDegrees() const;

  // Get the min pulse width in ms (ex: 1ms)
  float getMinPulseWidthInMs() const;

  // Get the max pulse width in ms (ex: 2ms)
  float getMaxPulseWidthInMs() const;

 private:
  PwmOut m_servoPwmOut;
  const float m_servoRangeInDegrees;
  const float m_minPulsewidthInMs;
  const float m_maxPulsewidthInMs;
  const float m_periodInMs = 20;
}