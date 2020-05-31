#ifndef TUTORIAL_SERVO_H
#define TUTORIAL_SERVO_H


#include "mbed.h"

class TutorialServo
{
   public: 

//COnstructor
   TutorialServo(PinName servoPin, float servoFrequency);

   void setAngleRangeInDegrees(float degrees);
   void setPulsewidthRangeInMs(float minPulsewidthMs, float maxPulsewidthMs);
   void setPositionInDegrees(float degrees);

   private:
   PwmOut servoPwm;

};

#endif