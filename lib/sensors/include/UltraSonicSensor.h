#pragma once 

#include "mbed.h"

#define TWO_MICRO_SECONDS 2000
#define TEN_MICRO_SECONDS 10000

class UltraSonicSensor{

public:
    // Constructor for UltrasonicSensor
    UltraSonicSensor(PinName echoPin, PinName trigPin);
    float getDistance();
    unsigned long pulseIn();

private: 

    DigitalIn _echoPin;
    DigitalOut _trigPin;
    unsigned long _duration;
    float _distance;
    // set up stopwatch 
    Timer _clock;
    // enum class to contain states of echoPin 
    class enum PinState{
        ECHO_PIN_LOW = 0,
        ECHO_PIN_HIGH = 1
    };

};