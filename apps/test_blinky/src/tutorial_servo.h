#ifndef TUTORIAL_SERVO_H
#define TUTORIAL_SERVO_H

#include "mbed.h"

class TutorialServo {

public:

   // Constructor: Takes a servo pin name (ex. PA_1), and optionally a servo range
   // that has a default value of 180.0 degrees
   TutorialServo(PinName servoPin, float servoRangeInDegrees);

   void setRangeInDegrees(float degrees);    // Set servo range (ex. 180 or 270 deg)
   void setPositionInDegrees(float degrees); // Set servo position (ex. 45 deg)

private:

   float  _rangeInDegrees; // default case for tutorial servo
   PwmOut _servoPwmOut;

};

#endif

