#include "TutorialServo.h"

TutorialServo::TutorialServo(PinName servoPin, float servoRangeInDegrees, float minPulsewidthInMs, 
			     float maxPulsewidthInMs)			                                
    : _servoPwmOut(servoPin),
      _minPulseWidthInMs(minPulsewidthInMs),
      _maxPulseWidthInMs(maxPulsewidthInMs),
      _servoRangeInDegrees(servoRangeInDegrees) { 
	      
  _minPulseWidthInMs = _minPulseWidthInMs / 1000;
  _maxPulseWidthInMs = _maxPulseWidthInMs / 1000;
  
  // default period of servo is 50Hz or 20 milliseconds 
  _servoPwmOut.period(0.020);
}

void TutorialServo::setAngleRangeInDegrees(float degrees) {
  
  _servoRangeInDegrees = degrees; 
	
}

void TutorialServo::setPulseWidthRangeInMs(float minPulseWidthInMs, float maxPulseWidthInMs) {
  _minPulseWidthInMs = minPulseWidthInMs / 1000;
  _maxPulseWidthInMs = maxPulseWidthInMs / 1000;
}


void TutorialServo::setServoPositionWithPercent(float percent) {
  // user cannot enter a percent bigger than 100
  percent = (percent > 100) ? 100 : percent;
  // user cannot enter a percent less than 0
  percent = (percent < 0) ? 0 : percent;
  float value;
  value = ((_maxPulseWidthInMs - _minPulseWidthInMs) / 100) * percent) + _minPulseWidthInMs;
  _servoPwmOut.pulsewidth(value);
}

void TutorialServo::setServoPositionInDegrees(float degrees) {	
  float value;

  value = (degrees / _servoRangeInDegrees) * 100;
  setServoPositionWithPercent(value);
  /* alternate method to setting servo position

  value = ((_maxPulseWidthInMs - _minPulseWidthInMs) / _servoRangeInDegrees) * degrees) + _minPulseWidthInMs;
  _servoPwmOut.pulsewidth(value);

  */
}
