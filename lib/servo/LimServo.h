#pragma once

#include "mbed.h"
#include "Servo.h"


class LimServo : public Servo {

public:
	LimServo(PinName pin);  
	LimServo(PinName pin, float range);   
	LimServo(PinName pin, float range, float max_pulse_ms, float min_pulse_ms);   


    bool setRange(float range); 
    float getRange(void); 

    bool setPosition(float pos);      
    float read(void);                  

protected:
	const int   DEFAULT_RANGE = 180;    //DEFAULT RANGE for LIMITED Servos

    float       m_range,                  //RANGE OF MOTION, only valid for LIMIT_SERVO types
                m_pos;                    //POSITION of servo

};