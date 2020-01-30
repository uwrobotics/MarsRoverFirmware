#pragma once

#include "mbed.h"
#include "Encoder.h"
#include "QEI.h"

class EncoderRelative_Quadrature: public Encoder {

public:

    EncoderRelative_Quadrature(t_encoderConfig config);
    ~EncoderRelative_Quadrature();

    // Must be implemented
    t_encoderType getType();
    float getAngle_Degrees();
    float getVelocity_DegreesPerSec();

    mbed_error_status_t reset();

private:

    QEI m_QEI;
    float m_degreesPerUnit;

};