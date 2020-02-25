#pragma once

#include "mbed.h"     
           
class Servo {

public:

enum SERVO_TYPE {CONT_SERVO = 0, LIM_SERVO = 1}; //CONT_SERVO = 0 = CONTINUOUS Rotation; LIMITED_SERVO = 1 = LIMITED Range Rotation 

protected:
    PinName pin;                                //PIN the servo PWM signal is attached to
    PwmOut pwm;                                 //PWM object, does not have default constructor so have to use INITIALIZER LIST to avoid COMPILER attempting to DEFAULT CONSTRUCT
    
    const int   PWM_FREQ = 50,                  //DEFAULT PWM FREQUENCY, should work for both length and positional control 
                DEFAULT_MAX = 2,                //DEFAULT MAX WAVE LENGTH in MILISECONDS
            DEFAULT_MIN = 1,                    //DEFAULT MIN WAVE LENGTH in MILISECONDS
                DEFAULT_RANGE = 180,            //DEFAULT RANGE for LIMITED Servos
                PERIOD = 1000 / PWM_FREQ;       //DEFAULT PERIOD LENGTH in MILISECONDS

    enum SERVO_TYPE rotate_type;                //Either CONTINUOUS or LIMITED rotation servo, see define statements.

        float       range,                      //RANGE OF MOTION, only valid for LIMIT_SERVO types
                    max_speed,                  //MAXIMUM ROTATION SPEED in ANGLES PER SECOND, only valid for CONTINUOUS_SERVO types
                    max_pulse_ms,               //PULSE LENGTH for MAX ANGLE 
                    min_pulse_ms,               //PULSE LENGTH for MIN ANGLE
                    pos,                        //POSITION of servo
                    speed;                      //ROTATING SPEED in ANGLES PER SECOND

    public:
    
        Servo(PinName pin_, SERVO_TYPE rotate_type_, float value, float max_pulse_ms_, float min_pulse_ms_);    //Copies previous constructors but with PIN
        Servo(PinName pin_, SERVO_TYPE rotate_type_);              
        Servo(PinName pin_, SERVO_TYPE rotate_type_, float value);    

        bool setRange(float range_);           //Returns FALSE if Servo Type is CONTINUOUS
        bool setMaxSpeed(float max_speed_);   //Returns FALSE if Servo Type is LIMITED
        
        bool setPosition(float angle);         //Sets POS to angle if Servo Type LIMITED
        bool setSpeed(float speed_);           //Sets rotation SPEED if Servo Type is CONTINUOUS

        float read(void);                      //Returns SPEED if CONTINUOUS and POSITION if LIMITED
        float getMaxSpeed(void);               //Returns maximum speed

        void setPeriod(int period);            //Override default period (ONLY USE FOR SPECIFIC FREQ REQUIREMENT)

        Servo operator=(Servo servo_);
};