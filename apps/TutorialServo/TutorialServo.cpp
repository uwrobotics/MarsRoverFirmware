/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "TutorialServo.h"
#include "mbed.h"
#include <cstdlib>


void TutorialServo::setAngleRangeInDegrees(float degrees)
{
  angleRange = degrees;
}

void TutorialServo::setPulseWidthRangeInMs(float minPulseWidthMsFloat, float maxPulseWidthMsFloat)
{
  minPulseWidthMs = minPulseWidthMsFloat;
  maxPulseWidthMs = maxPulseWidthMsFloat;
}

void TutorialServo::setPositionInDegrees(float degrees)
{
  if(!((std::fabs(degrees) > angleRange)))
    {
        double pulseWidthMs = 1.5 + 0.5*degrees/angleRange; //Sets the pulse width that will be used, as 1ms is the lower bound and 2ms is the upper bound.
        //1.5 is neutral and degrees can be either positive or negative which can make a pulse with from 1-2ms
        if((minPulseWidthMs < pulseWidthMs) && (pulseWidthMs < maxPulseWidthMs)) //checks to ensure it is within bounds
        {
           servoPwm.pulsewidth(pulseWidthMs/1000); //set pulse width
        }     
        
    }
}

TutorialServo::TutorialServo(PinName servoPin, float servoFrequency) : servoPwm(servoPin) //Constructor
{
  servoPwm.period(1/servoFrequency);
  
  //Set default parameters:
  minPulseWidthMs = 1;
  maxPulseWidthMs = 2;
  angleRange = 90; //This is the absolute value of the angle range
}