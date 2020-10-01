#include "UltraSonicSensor.h"

UltraSonicSencor::UltraSonicSensor(PinName echoPin, PinName trigPin): _echoPin(echoPin), _trigPin(trigPin){
    
}

#define _trigPin.write(0) TRIG_PIN_LOW
#define _trigPin.write(1) TRIG_PIN_HIGH
#define _echoPin.read() ECHO_PIN_READ

int ECHO_PIN_STATE = (_echoPin.read() == 0)? PinState::ECHO_PIN_LOW : PinState::ECHO_PIN_HIGH;

unsigned long UltraSonicSensor::pulseIn(){
    unsigned long _time{};
    // reset timer before it starts
    _clock.reset();
    if(ECHO_PIN_STATE == PinState::ECHO_PIN_HIGH){
        // start timer
        _clock.start();
        while(_ECHO_PIN_READ == PinSate::ECHO_PIN_HIGH){
            if(_ECHO_PIN_READ == PinState::ECHO_PIN_LOW){break;}
        }
        
    }
    // stop the timer because _echoPin is now Logic LOW
    _clock.stop();
    // get the duration of time of echoPin in High State in microseconds
    _time = _clock.read_us();
    return _time;
    
}

float UltraSonicSensor::getDistance(){
    // write to trigPin low
    TRIG_PIN_LOW
    // wait 10 microseconds or 10000 nanoseconds
    wait_ns(TWO_MICRO_SECONDS);
    // write trigPin high
    TRIG_PIN_HIGH
    // wait 2 microseconds or 2000 nanoseconds
    wait_ns(TEN_MICRO_SECONDS);
    // write trigPin low
    TRIG_PIN_LOW

    _duration = pulseIn();
    // get duration from pulseIn() function
    _distance = _duration * 0.034/2;

    return _distance;

}




