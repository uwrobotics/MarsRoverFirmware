#ifndef TUTORIAL_SERVO_H
#define TUTORIAL_SERVO_H

class TutorialServo {

public:

   // Constructor: Takes a servo pin name (ex. PA_1), and optionally a servo range
   // that has a default value of 180.0 degrees, a minimum pulsewidth of 1ms, and a     
   // maximum pulsewidth of 2ms.
   TutorialServo(PinName servoPin, float servoRangeInDegrees = 180.0, 
                 float minPulsewidthInMs = 1, float maxPulsewidthInMs = 2); 

   // Set servo range (ex. 270 deg)
   void setAngleRangeInDegrees(float degrees); 

   // Set servo pulse width range (ex. 1ms, 2ms)
   void setPulsewidthRangeInMs(float minPulsewidthMs, float maxPulsewidthMs);

   // Set servo position (ex. 45 deg)
   void setPositionInDegrees(float degrees); 

private:

   PwmOut m_servoPwmOut;
   float  m_servoRangeInDegrees;
   float  m_minPulsewidthInMs;
   float  m_maxPulsewidthInMs;
   
};

#endif
