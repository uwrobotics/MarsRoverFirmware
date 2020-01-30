#pragma once

#ifndef PWM_IN_DEFAULT_NUM_SAMPLES_TO_AVERAGE
#define PWM_IN_DEFAULT_NUM_SAMPLES_TO_AVERAGE 12
#endif

#include "Encoder.h"
#include "PwmIn.h"

class EncoderAbsolute_PWM: public Encoder {

public:

    EncoderAbsolute_PWM(t_encoderConfig config);
    ~EncoderAbsolute_PWM();

    // Must be implemented
    t_encoderType getType();
    float getAngle_Degrees();
    float getVelocity_DegreesPerSec();

private:

    PwmIn m_pwmIn;
    float m_degreesPerUnit;

};