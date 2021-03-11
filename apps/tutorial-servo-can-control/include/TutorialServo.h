#pragma once

#include "mbed.h"

class Tutorial Servo {
  public:
    TutorialServo(PinName servoPin, float servoRangeInDegrees = 180.0, float minPulsewidthInMs = 1, float maxPulsewidthInMs = 2);
    
    // Set servo position to a certain angle
    void setPositionInDegrees(const float degrees);
    
    // Get servo range in degrees
    float getServoRangeInDegrees() const;
    
    // Get the min pulse width in ms
    float getMinPulsewidthInMs() const;
    
    // Get the max pulse width in ms
    float getMaxPulsewidthInMs() const;
  
  private:
    const float m_servoRangeInDegrees;
    const float m_minPulsewidthInMs;
    const float m_maxPulsewidthInMs;
    
};
