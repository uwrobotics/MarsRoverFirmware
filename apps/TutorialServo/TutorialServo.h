#ifndef TUTORIAL_SERVO_H
#define TUTORIAL_SERVO_H


#include "mbed.h"

class TutorialServo
{
   public: 

//COnstructor
      TutorialServo(PinName servoPin, float servoFrequency);

      void setAngleRangeInDegrees(float degrees);
      void setPulsewidthRangeInMs(float minPulsewidthMsFloat, float maxPulsewidthMsFloat);
      void setPositionInDegrees(float degrees);

    };
 PwmOut servoPwm;

#endif