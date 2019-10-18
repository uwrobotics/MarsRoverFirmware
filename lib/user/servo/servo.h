#ifndef SERVO_H
#define SERVO_H

#include "mbed.h"

#define CONT_SERVO = 0                      //CONTINUOUS Rotation
#define LIMITED_SERVO = 1                     //LIMITED Range Rotation

#define PWM_FREQ = 50                       //DEFAULT PWM FREQUENCY, should work for both length and positional control
#define DEFAULT_MAX = 2                     //DEFAULT MAX WAVE LENGTH in MILISECONDS
#define DEFAULT_MIN = 1                     //DEFAULT MIN WAVE LENGTH in MILISECONDS
#define DEFAULT_RANGE = 180                 //DEFAULT RANGE for LIMITED Servos

#define NULL = -1                           //PLACEHOLDER for irrelevant values in constructors

    class Servo{
        private:
            PinName pin;                    //PIN the servo PWM signal is attached to

            int     rotate_type,            //Either CONTINUOUS or LIMITED rotation servo, see define statements.
                    range;                  //range of motion, only valid for LIMIT_SERVO types

            float   max_pulse_ms,           //PULSE LENGTH for MAX ANGLE 
                    min_pulse_ms,           //PULSE LENGTH for MIN ANGLE
                    pos,                    //POSITION of servo
                    speed;                  //ROTATING SPEED in ANGLES PER SECOND

            const int PERIOD = 1000 / PWM_FREQ; //DEFAULT PERIOD LENGTH in MILISECONDS

        public:
            Servo(int rotate_type_, int range_, float max_pulse_ms_, float min_pulse_ms_);
            Servo(int rotate_type_);                //If LIMITED, ASSUMING DEFAULT MAX, MIN, RANGE
            Servo(int rotate_type_, int range_);    //If LIMITED, ASSUMING DEFAULT MAX, MIN

            bool set_range(int range);      //Returns FALSE if Servo Type is CONTINUOUS
            bool move(float angle);         //Returns FALSE if ANGLE > RANGE or Servo Type is CONTINUOUS
            bool set_speed(float speed);    //Returns FALSE if Servo Type is LIMITED

            void attach(PinName pin_);

            float read(void);               //Returns SPEED if CONTINUOUS and POSITION if LIMITED
    }
#endif