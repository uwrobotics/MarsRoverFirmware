#include "EncoderAbsolute_PWM.h"

EncoderAbsolute_PWM::EncoderAbsolute_PWM(t_encoderConfig encoderConfig) :
		Encoder(encoderConfig), m_encoderType(encoderConfig.encoderType), m_pwmIn(encoderConfig.encoderPins[0]) {}

Encoder::t_encoderType EncoderAbsolute_PWM::getType() {
	return m_encoderType;
}

float EncoderAbsolute_PWM::getAngle_Degrees() {
	return m_pwmIn.avgDutyCycle() * 360.0;
}

float EncoderAbsolute_PWM::getVelocity_DegreesPerSec() {
	return m_pwmIn.avgDutyCycleVelocity() * 360.0;
}