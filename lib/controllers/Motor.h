#pragma once

#ifndef MOTOR_DEFAULT_FREQUENCY_HZ
#define MOTOR_DEFAULT_FREQUENCY_HZ 244  // 10 kHz
#endif

#include "mbed.h"
#include "ContServo.h"
#include "LimServo.h"
 
// Interface to control a standard DC motor with an H-bridge using a PwmOut and 2 DigitalOuts

class Motor {

public:
    enum t_motorType {
        motor,
        lim_servo,
        cont_servo
    } motorType;

    typedef struct motorConfig{
        PinName pwmPin;
        PinName dirPin;
        bool inverted;
        int freqInHz;
        float limit;
    } t_motorConfig;
 
    /** Create a motor control interface    
     *
     * @param pwm       A PwmOut pin, driving the H-bridge enable line to control the speed
     * @param dir       A DigitalOut, set high when the motor should go forward, low when backwards
     * @param freqInHz  Output PWM frequency, default 1kHz
     * @param inverted  If true, then forward speed will set dir to 0 instead of 1, otherwise inverse
     * @param limit     Maximum speed magnitude
     */
    Motor(PinName pwm, PinName dir, bool inverted = false, int freqInHz = MOTOR_DEFAULT_FREQUENCY_HZ, float limit = 1.0, t_motorType motorType = motor);

    Motor(t_motorConfig motorConfig, t_motorType motorType = motor);

    ~Motor();

    /** Set the speed of the motor
     * 
     * @param dutyCycle The speed of the motor as a normalised value between -1.0 and 1.0
     */
    void setPower(float dutyCycle);
    Motor& operator=(int dutyCycle);

    /** Read the current speed of the motor
     * 
     * @return Current speed of motor
     */
    float getPower();

    /** Read the motor type
     * 
     * @return Motor type
     */
    t_motorType getType();
    
    // Servo type exclusive functions
    bool servoSetRange(float range_);           //Returns FALSE if t_motorType != lim_servo
    bool servoSetMaxSpeed(float max_speed_);   //Returns FALSE if t_motorType != cont_servo
    bool servoSetPosition(float angle);         //Sets POS to angle if t_motorType == lim_servo
    bool servoSetSpeed(float speed_);           //Sets rotation SPEED to speed_ if t_motorType == cont_servo
    float servoRead(void);                       //Returns SPEED if CONTINUOUS and POSITION if LIMITED
    float servoGetMaxSpeed(void);               //Returns MAX SPEED
    void servoSetPeriod(int period);            //Override default period (ONLY USE FOR SPECIFIC FREQ REQUIREMENT)

protected:
    PwmOut m_pwm;
    DigitalOut m_dir;
    bool m_inverted;
    float m_limit;
    
    // member servo, only used when t_motorType = servo
    Servo* m_servo;
};