#include "mbed.h"

#include "Encoder.h"
#include "EncoderAbsolute_PWM.h"
#include "ActuatorController.h"
#include "CANMsg.h"
#include "CANBuffer.h"
#include "Servo.h"
#include "PwmOut.h"

#define TIME_ERROR_FACTOR 3.15776

// Init. Components

PwmOut tilt (PB_15);
PwmOut pan (PB_14);

int max_width_us = 2.1*1000*TIME_ERROR_FACTOR,
    min_width_us = 0.9*1000*TIME_ERROR_FACTOR;
        
int main(){
    tilt.period_ms(20*TIME_ERROR_FACTOR);
    pan.period_ms(20*TIME_ERROR_FACTOR);

    pan.pulsewidth_us(max_width_us);
    tilt.pulsewidth_us(max_width_us);

    while(1){
        tilt.pulsewidth_us(max_width_us);
        wait_ms(500);
        tilt.pulsewidth_us(min_width_us);
        wait_ms(500);
    }
}