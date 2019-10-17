#ifndef SERVO_H
#define SERVO_H

#include mbed.h

#define CONT_SERVO = 0              //CONTINUOUS Rotation
#define LIMIT_SERVO = 1             //LIMITED Range Rotation

#define LENGTH_CTRL = 0             //PULSE LENGTH Positional Control; Position scales with length of waves, 40Hz < frequency < 200Hz is acceptable [defaulting to 50Hz]
#define DUTY_CTRL = 1               //DUTY CYCLE Positional Control; Position scales with on-time of PWM wave, frequency is set at 50Hz
#define PWM_FREQ = 50               //DEFAULT PWM FREQUENCY, should work for both length and positional control

#define NULL = -1                   //PLACEHOLDER for irrelevant values in constructors

    class Servo{
        private:
            int     rotate_type,        //Either CONTINUOUS or LIMITED rotation servo, see define statements.
                    control_type,       //If LENGTH_CTRL, linearly scale using pulse_ms; if DUTY_CTRL, linearly scale using duty cycle
                                        //Typically duty cycle maps 10% to MAX and 5% to MIN of range; Assuming so unless duty cycle is explicitly stated

                    range;              //range of motion, only valid for LIMIT_SERVO types

            float   max_pulse_ms,       //PULSE LENGTH for MAX ANGLE 
                    min_pulse_ms,       //PULSE LENGTH for MIN ANGLE
                    max_cycle,          //DUTY CYCLE for MAX ANGLE
                    min_cycle;          //DUTY CYCLE for MIN ANGLE

            const int PERIOD = 1000 / PWM_FREQ; //DEFAULT PERIOD LENGTH in MILISECONDS

        public:
            Servo(int rotate_type_, int control_type_, int range_, float max_pulse_ms_, float min_pulse_ms_, float max_cycle_, float min_cycle_);
            Servo(int rotate_type_, int control_type_);
            Servo(int rotate_type_, int control_type_, int range_);

            bool set_range(int range);      //Returns FALSE if Servo Type is CONTINUOUS
            bool move(float angle);         //Returns FALSE if ANGLE > RANGE or Servo Type is CONTINUOUS
    }

#endif