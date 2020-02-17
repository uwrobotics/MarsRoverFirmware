#include "mbed.h"
#include "EncoderAbsolute_PWM.h"

Encoder::t_encoderConfig config = {
    // PWM encoder pins
    .pin_PWM = ENC_PWM_SHLDR,

    .degreesPerUnit = 1.0,
    .inverted = false
};

EncoderAbsolute_PWM encoder(config);

int main () {
    while (true) {
        printf("Angle: %f, Angular Velocity :%f\r\n", encoder.getAngle_Degrees(), encoder.getVelocity_DegreesPerSec());
    }
}