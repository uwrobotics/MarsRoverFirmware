#pragma once

#include "Motor.h"
#include "ContServo.h"
 
// Control a continuous rotation servo like a motor

class ServoMotor : Motor {
 public:
  /** Create a motor control interface
   *
   * @param pwm       A PwmOut pin, driving the H-bridge enable line to control the speed
   * @param dir       A DigitalOut, set high when the motor should go forward, low when backwards [NOT USED BY
   * SERVOMOTOR CLASS]
   * @param freqInHz  Output PWM frequency, default 1kHz [NOT USED BY SERVOMOTOR CLASS]
   * @param inverted  If true, then forward speed will set dir to 0 instead of 1, otherwise inverse
   * @param limit     Maximum speed magnitude
   */
  ServoMotor(PinName pwm, bool inverted = false, float min_pulsewidth_ms = 1.0, float max_pulsewidth_ms = 2.0,
             float limit = 1.0);

  ServoMotor(Motor::t_motorConfig motorConfig);

  /** Set the speed of the motor
   *
   * @param dutyCycle The speed of the motor as a normalised value between -1.0 and 1.0
   */
  void setPower(float dutyCycle);
  ServoMotor& operator=(int dutyCycle);

    /** Set the speed of the motor
     * 
     * @param percentage The speed of the motor as a normalised value between -1.0 and 1.0
     */
    void setPower(float percentage);
    ServoMotor& operator=(int percentage);

    /** Read the current speed of the motor
     * 
     * @return Current speed of motor
     */
    float getPower();

     // ContServo type exclusive functions
    bool servoSetMaxSpeed(float max_speed_);   
    bool servoSetSpeed(float speed_);           
    float servoRead(void);                      
    float servoGetMaxSpeed(void);               
    void servoSetPeriod(int period);          

protected:
 
protected:
    bool m_inverted;
    float m_limit;
    ContServo m_servo;
};