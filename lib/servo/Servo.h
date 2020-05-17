#pragma once
 
#include "mbed.h"
#include "PwmOut.h"
#include "mbed.h"

class Servo {
  
public:
    
    enum t_servo_type {
        CONT_SERVO,
        LIM_SERVO
    } servo_type;  

protected:
    PinName m_pin;                    //PIN the servo PWM signal is attached to
    PwmOut m_pwm;                     //PWM object, does not have default constructor so have to use INITIALIZER LIST to avoid COMPILER attempting to DEFAULT CONSTRUCT
    
    const int    PWM_FREQ = 50,          //DEFAULT PWM FREQUENCY, should work for both length and positional control 
                 DEFAULT_MAX = 2,        //DEFAULT MAX WAVE LENGTH in MILISECONDS
                 DEFAULT_MIN = 1,        //DEFAULT MIN WAVE LENGTH in MILISECONDS
                 PERIOD = 1000 / PWM_FREQ; //DEFAULT PERIOD LENGTH in MILISECONDS

    t_servo_type m_rotate_type;          //Either CONTINUOUS or LIMITED rotation servo, see define statements.

    float        m_max_pulse_ms,           //PULSE LENGTH for MAX ANGLE 
                 m_min_pulse_ms;           //PULSE LENGTH for MIN ANGLE

    int getSign(int val) { 
        return (val >= 0) ? 1 : -1; 
    }

    public:
        Servo(PinName pin);            
        virtual ~Servo();
        virtual bool setRange(float range);   // Returns FALSE if Servo Type is CONTINUOUS   
        virtual bool setMaxSpeed(float max_speed);  // Returns FALSE if Servo Type is LIMITED
        virtual float getRange(void);         // Returns -1 if Servo Type is CONTINUOUS   
        virtual float getMaxSpeed(void);      // Returns -1 if Servo Type is LIMITED   
        
        virtual bool setPosition(float angle);  // Returns FALSE if Servo Type is CONTINUOUS  
        virtual bool setSpeed(float speed);     // Returns FALSE if Servo Type is LIMITED

        virtual float read(void);            //Returns SPEED if CONTINUOUS and POSITION if LIMITED, and -1 if calling from base class

        void setPeriod(int period);         //Override default period (ONLY USE FOR SPECIFIC FREQ REQUIREMENT)
};
