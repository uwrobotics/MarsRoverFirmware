#ifndef SERVO_H
#define SERVO_H

#include "mbed.h"

#define CONT_SERVO 0                        //CONTINUOUS Rotation
#define LIMITED_SERVO 1                     //LIMITED Range Rotation

#define PWM_FREQ 50                         //DEFAULT PWM FREQUENCY, should work for both length and positional control
#define DEFAULT_MAX 2                       //DEFAULT MAX WAVE LENGTH in MILISECONDS
#define DEFAULT_MIN 1                       //DEFAULT MIN WAVE LENGTH in MILISECONDS
#define DEFAULT_RANGE 180                   //DEFAULT RANGE for LIMITED Servos

#define NULL -1                           //PLACEHOLDER for irrelevant values in constructors

    class Servo{
        protected:
            PinName pin;                    //PIN the servo PWM signal is attached to
            PwmOut pwm;                     //PWM object, does not have default constructor so have to use INITIALIZER LIST to avoid COMPILER attempting to DEFAULT CONSTRUCT

            int     rotate_type;            //Either CONTINUOUS or LIMITED rotation servo, see define statements.


            float   range,                  //RANGE OF MOTION, only valid for LIMIT_SERVO types
                    max_speed,              //MAXIMUM ROTATION SPEED in ANGLES PER SECOND, only valid for CONTINUOUS_SERVO types
                    max_pulse_ms,           //PULSE LENGTH for MAX ANGLE 
                    min_pulse_ms,           //PULSE LENGTH for MIN ANGLE
                    pos,                    //POSITION of servo
                    speed;                  //ROTATING SPEED in ANGLES PER SECOND

            const int PERIOD = 1000 / PWM_FREQ; //DEFAULT PERIOD LENGTH in MILISECONDS

        public:
            Servo(PinName pin_, int rotate_type_, float val, float max_pulse_ms_, float min_pulse_ms_);    //Copies previous constructors but with PIN
            Servo(PinName pin_, int rotate_type_);              
            Servo(PinName pin_, int rotate_type_, float val);    

            bool set_range(float range_);       //Returns FALSE if Servo Type is CONTINUOUS
            bool set_max_speed(float max_speed_);   //Returns FALSE if Servo Type is LIMITED
            
            bool move(float angle);             //Sets POS to angle if Servo Type LIMITED
            bool set_speed(float speed_);       //Sets rotation SPEED, where M

            float read(void);                   //Returns SPEED if CONTINUOUS and POSITION if LIMITED

            void set_period(int period);         //Override default period (ONLY USE FOR SPECIFIC FREQ REQUIREMENT)

        /*  Servo(int rotate_type_, int range_, float max_pulse_ms_, float min_pulse_ms_);
            Servo(int rotate_type_);                //If LIMITED, ASSUMING DEFAULT MAX, MIN, RANGE
            Servo(int rotate_type_, int range_);    //If LIMITED, ASSUMING DEFAULT MAX, MIN */
    };

#endif