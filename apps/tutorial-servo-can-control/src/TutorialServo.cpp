#include "TutorialServo.h"
#include "mbed.h"


TutorialServo::TutorialServo(PinName servoPin, float servoRangeInDegrees, float minPulsewidthInMs ,
              float maxPulsewidthInMs)
                  : m_servoPwmOut(servoPin), m_servoRangeInDegrees(servoRangeInDegrees),
                    m_minPulsewidthInMs(minPulsewidthInMs),
                    m_maxPulsewidthInMs(maxPulsewidthInMs) {}

void TutorialServo::setPositionInDegrees(const float degrees) {
  //Ensure degree entered is within the range specified in m_servoRangeInDegrees
   float fixValue(float value, float minValue, float maxValue){
    if(value< minValue){
      return minValue;
    }
    else if(value> maxValue){
      return maxValue;
    }
    else{
      return value;}
  }
  // Set the pulse width on the PWM Pin using the degrees entered and
  //    ensure pulsewidth is within the max and min Pulse width values
// figure out the function for the degrees/ pulse width line to use for the pulsewidth function
  // float pWidth = m_servoPwmOut.pulsewidth(m_minPulsewidthInMs + (fixValue(degrees,
  //     0, m_servoRangeInDegrees)/m_servoRangeInDegrees));
  float pWidth = m_servoPwmOut.pulsewidth_ms(((m_maxPulsewidthInMs-m_minPulsewidthInMs)
    /m_servoRangeInDegrees)*fixValue(degrees,
      0, m_servoRangeInDegrees)+m_minPulsewidthInMs)
//The above code shoudl work now. Check changes - Gb
//
  // float pWidthCalc(float i){
//        if((i*1000)<m_minPulsewidthInMs){
//          return m_minPulsewidthInMs;
//         }
//         else if((i*1000)>m_maxPulsewidthInMs){
//           return m_maxPulsewidthInMs;
//         }
//         else{
//           return pWidth;
//         }
//   }
//   m_servoPwmOut.pulsewidth(pWidthCalc(pWidth));
 }

float getServoRangeInDegrees() const { return m_servoRangeInDegrees;}

float getMinPulseWidthInMs() const { return m_minPulsewidthInMs;}

float getMaxPulseWidthInMs() const { return m_maxPulsewidthInMs;}