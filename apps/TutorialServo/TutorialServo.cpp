#include "TutorialServo.h"
#include "mbed.h"

float minPulsewidthInMs, maxPulsewidthInMs;
PwmOut servoPwm;
float angleRange = 180;

TutorialServo::TutorialServo(PinName servoPin, float servoRangeInDegrees, float servoFrequency)
{
    servoPwm.period(1/servoFrequency);
    servoPwm(servoPin);
}

void TutorialServo::setAngleRangeInDegrees(float degrees)
{
    angleRange = degrees;
}

   // Set servo pulse width range (ex. 1ms, 2ms)
void TutorialServo::setPulsewidthRangeInMs(float minPulsewidthMs, float maxPulsewidthMs)
{
    this.minPulsewithInMs = minPulsewidthInMs;
    this.maxPulsewidthInMs = maxPulsewidthInMs;
}
   // Set servo position (ex. 45 deg)
void TutorialServo::setPositionInDegrees(float degrees)
{  
    if(!(degrees > angleRange))
    {
        double pulseWidthMs = 1 + degrees/angleRange;
        if(minPulsewidthInMs < pulseWidth < maxPulsewidthInMs)
        {
            servoPwm.pulseWidth(pulseWidthMs/1000); //set pulse width
        }     
        
    }

}