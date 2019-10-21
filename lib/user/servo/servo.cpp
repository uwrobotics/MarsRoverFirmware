#include "servo.h"
#include "mbed.h"

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
    }
    else
    {
        max_pulse_ms = NULL;
        min_pulse_ms = NULL;
        range = NULL;
    }
}

bool Servo::move(float angle)
{
    if(rotate_type != LIMITED_SERVO)
        return false;
    else
    {
        
    }
}
