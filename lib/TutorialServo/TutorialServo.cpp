
#include "TutorialServo.h"

TutorialServo::TutorialServo(PinName servoPin, float servoRangeInDegrees, float minPulsewidthInMs,
                float maxPulsewidthInMs): m_servoPwmOut(servoPin),
                              m_servoRangeInDegrees(servoRangeInDegrees), 
                              m_minPulsewidthInMs(minPulsewidthInMs),
                              m_maxPulsewidthInMs(maxPulsewidthInMs) {
  // setting the member variables using initializer list
  // otherwise nothing to do
}

TutorialServo::~TutorialServo() {
  m_servoRangeInDegrees = 0;
  m_minPulsewidthInMs   = 0;
  m_maxPulsewidthInMs   = 0;
  // felt i should include a destructor although seems trivial
}

void TutorialServo::setAngleRangeInDegrees(float degrees) {
  m_servoRangeInDegrees = degrees;
  // updating the member variable not sure if there is anything else to do here
}

void TutorialServo::setPulsewidthRangeInMs(float minPulsewidthMs, float maxPulsewidthMs) {
  m_minPulsewidthInMs = minPulsewidthMs;
  m_maxPulsewidthInMs = maxPulsewidthMs;
  // updating the member variables (not sure if there is anything else to do here)
}

void TutorialServo::setPositionInDegrees(float degrees) {
  m_servoPwmOut.pulsewidth_us(int(((m_maxPulsewidthInMs - m_minPulsewidthInMs) * degrees / 180 + m_minPulsewidthInMs) * 1000));
  // setting the servo position based of servo.cpp in lib subforlder
}
