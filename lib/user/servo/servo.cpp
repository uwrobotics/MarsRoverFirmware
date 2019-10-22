#include "servo.h"
#include "mbed.h"
#include "PwmOut.h"

/*
Servo::Servo(int rotate_type_, int range_, float max_pulse_ms_, float min_pulse_ms_): 
rotate_type(rotate_type_),
range(range_),
max_pulse_ms(max_pulse_ms_),
min_pulse_ms(min_pulse_ms_) {}

Servo::Servo(int rotate_type_, int range_):
rotate_type(rotate_type_),
range(range_)
{
    if(rotate_type == LIMITED_SERVO)
    {
        max_pulse_ms = DEFAULT_MAX;
        min_pulse_ms = DEFAULT_MIN;
    }
    else
    {
        max_pulse_ms = NULL;
        min_pulse_ms = NULL;
    }
}

Servo::Servo(int rotate_type_):
rotate_type(rotate_type_)
{
    if(rotate_type == LIMITED_SERVO)
    {
        max_pulse_ms = DEFAULT_MAX;
        min_pulse_ms = DEFAULT_MIN;
        range = DEFAULT_RANGE;
    }
    else
    {
        max_pulse_ms = NULL;
        min_pulse_ms = NULL;
        range = NULL;
    }
}
*/

Servo::Servo(PinName pin_, int rotate_type_, int range_, float max_pulse_ms_, float min_pulse_ms_): 
pin(pin_),
rotate_type(rotate_type_),
range(range_),
max_pulse_ms(max_pulse_ms_),
min_pulse_ms(min_pulse_ms_) {
    
}

Servo::Servo(PinName pin_, int rotate_type_, int range_):
pin(pin_),
rotate_type(rotate_type_),
range(range_)
{
    if(rotate_type == LIMITED_SERVO)
    {
        max_pulse_ms = DEFAULT_MAX;
        min_pulse_ms = DEFAULT_MIN;
    }
    else
    {
        max_pulse_ms = NULL;
        min_pulse_ms = NULL;
    }
}

Servo::Servo(PinName pin_, int rotate_type_):
pin(pin_),
rotate_type(rotate_type_)
{
    if(rotate_type == LIMITED_SERVO)
    {
        max_pulse_ms = DEFAULT_MAX;
        min_pulse_ms = DEFAULT_MIN;
        range = DEFAULT_RANGE;

        //INIT PWM
        PwmOut pwm(pin_);
        pwm.period_ms(1000/PWM_FREQ);
    }
    else
    {
        max_pulse_ms = NULL;
        min_pulse_ms = NULL;
        range = NULL;
        
        //INIT PWM
        PwmOut pwm(pin_);
        pwm.period_ms(1000/PWM_FREQ);
    }
}

bool Servo::move(float angle)
{
    if(rotate_type != LIMITED_SERVO)
        return false;
    else
    { 
        pos = angle;
        pwm.pulsewidth_ms(int((max_pulse_ms - min_pulse_ms) * angle/180));
    }
}

float read(void)
{
    return Servo::pos;
}
