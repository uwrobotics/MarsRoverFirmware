#include "TutorialServo.h"
#include "mbed.h"

float minPulsewidthInMs, maxPulsewidthInMs;
float angleRange = 180;
//PwmOut servoPwm;



void TutorialServo::setAngleRangeInDegrees(float degrees)
{
    angleRange = degrees;
}

   // Set servo pulse width range (ex. 1ms, 2ms)
void TutorialServo::setPulsewidthRangeInMs(float minPulsewidthMsFloat, float maxPulsewidthMsFloat)
{
    minPulsewidthInMs = minPulsewidthMsFloat;
    maxPulsewidthInMs = maxPulsewidthMsFloat;
}
   // Set servo position (ex. 45 deg)
void TutorialServo::setPositionInDegrees(float degrees)
{  
    if(!(degrees > angleRange))
    {
        double pulseWidthMs = 1 + degrees/angleRange;
        if((minPulsewidthInMs < pulseWidthMs) && (pulseWidthMs < maxPulsewidthInMs))
        {
           servoPwm.pulsewidth(pulseWidthMs/1000); //set pulse width
        }     
        
    }

}

TutorialServo::TutorialServo(PinName servoPin, float servoFrequency) : servoPwm(servoPin)
{
    servoPwm.period(1/servoFrequency);
    
}