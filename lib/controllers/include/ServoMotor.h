#pragma once  // TODO(qinyang-bao): fix all compile errors and renable servo code

#include "Motor.h"
#include "Servo.h"
#include "mbed.h"

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
  ServoMotor(PinName pwm, bool inverted = false, double min_pulsewidth_ms = 1.0, double max_pulsewidth_ms = 2.0,
             double limit = 1.0);

  ServoMotor(Motor::t_motorConfig motorConfig);

  /** Set the speed of the motor
   *
   * @param dutyCycle The speed of the motor as a normalised value between -1.0 and 1.0
   */
  void setPower(double dutyCycle);
  ServoMotor& operator=(int dutyCycle);

  /** Read the current speed of the motor
   *
   * @return Current speed of motor
   */
  double getPower();

 protected:
  Servo m_servo;
  bool m_inverted;
  double m_limit;
};
