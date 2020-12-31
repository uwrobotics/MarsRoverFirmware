#pragma once  
#include "mbed.h"  
class TutorialServo {  
public :  
 // Constructor: Takes a servo pin name (ex. PA_1), and optionally a servo range   // that has a default value of 180.0 degrees, a minimum pulsewidth of 1ms, and a   // maximum pulsewidth of 2ms. 


 TutorialServo(PinName servoPin, float servoRangeInDegrees = 180.0,   float minPulsewidthInMs = 1, float maxPulsewidthInMs = 2) ;  
 // Set servo position (ex. 45 deg)  
 void setPositionInDegrees( const float degrees) ;  
 // Get the servo range in degrees (ex: 90 deg)  
 float getServoRangeInDegrees( ) const ;  
 // Get the min pulse width in ms (ex: 1ms)  
 float getMinPulseWidthInMs( ) const ;  
 // Get the max pulse width in ms (ex: 2ms)  
 float getMaxPulseWidthInMs( ) const ;  
private :  
 PwmOut m_servoPwmOut ;  
 const float m_servoRangeInDegrees ;  
 const float m_minPulsewidthInMs;  
 const float m_maxPulsewidthInMs;  
} ; 
