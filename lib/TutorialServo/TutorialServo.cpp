
#include "TutorialServo.h"

TutorialServo::TutorialServo(PinName servoPin, float servoRangeInDegrees, float minPulsewidthInMs,
                             float maxPulsewidthInMs)
    : m_servoPwmOut(servoPin),
      m_servoRangeInDegrees(servoRangeInDegrees),
      m_minPulsewidthInMs(minPulsewidthInMs),
      m_maxPulsewidthInMs(maxPulsewidthInMs) {
  m_servoPwmOut.period(0.020);
  // setting the member variables using initializer list
  // otherwise nothing to do
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

void TutorialServo::setPositionInDegrees(float degrees) {  // function takes in a non-negative angle
  // 0 will represent the min pulse width
  // range / 2 will represent the default position of servo
  // range represents the total range of motion between min pulse and max pulse
  if (degrees >= m_servoRangeInDegrees) {  // checking if the desired angle is in range
                                           // the desired angle is greater than what is allowed
    m_servoPwmOut.pulsewidth_us(m_maxPulsewidthInMs * 1000);
  } else if (degrees <= 0) {
    // degrees are less than 0
    m_servoPwmOut.pulsewidth_us(m_minPulsewidthInMs * 1000);
  } else {
    m_servoPwmOut.pulsewidth_us(int(
        ((m_maxPulsewidthInMs - m_minPulsewidthInMs) * degrees / m_servoRangeInDegrees + m_minPulsewidthInMs) * 1000));
  }
  // setting the servo position based of servo.cpp in lib subforlder
}

float TutorialServo::returnAngleRangeInDegrees() {
  return m_servoRangeInDegrees;
}
