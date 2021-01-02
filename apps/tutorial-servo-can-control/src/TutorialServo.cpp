#include "TutorialServo.h"
#include <algorithm>

TutorialServo::TutorialServo(PinName servoPin, float servoRangeInDegrees = 180.0,
						float minPulsewidthInMs = 1, float maxPulsewidthInMs = 2)
	:	m_servoPwmOut(servoPin),
		m_servoRangeInDegrees(servoRangeInDegrees),
		m_minPulsewidthInMs(minPulsewidthInMs),
		m_maxPulsewidthInMs(maxPulsewidthInMs) {}

void TutorialServo::setPositionInDegrees(const float degrees) {
	float validated_angle = max(0.0, min(servoRangeInDegrees, degrees));
	m_servoPwmOut = minPulsewidthInMs + (validated_angle / servoRangeInDegrees) * maxPulsewidthInMs;
}

float TutorialServo::getServoRangeInDegrees() const {
	return m_servoRangeInDegrees;
}

float TutorialServo::getMinPulseWidthInMs() const {
	return m_minPulsewidthInMs;
}

float TutorialServo::getMaxPulseWidthInMs() const {
	return m_maxPulsewidthInMs;
}

