#pragma once

#include "ContServo.h"
#include "Motor.h"
#include "mbed.h"

// Control a continuous rotation servo like a motor

class ServoMotor : public Motor {
 public:
  /** Create a motor control interface
   *
   * @param pwm       A PwmOut pin, driving the H-bridge enable line to control the speed
   * @param inverted  If true, then forward speed will set dir to 0 instead of 1, otherwise inverse
   * @param min_pulsewidth_ms  Pwm pulse width for minimum speed
   * @param max_pulsewidth_ms  Pwm pulse width for maximum speed
   * @param limit     Maximum speed magnitude
   */
  ServoMotor(PinName pwm, bool inverted = false, double min_pulsewidth_ms = 1.0, double max_pulsewidth_ms = 2.0,
             double limit = 1.0);

  ServoMotor(Motor::t_motorConfig motorConfig);

  /** Set the speed of the motor
   *
   * @param percentage The speed of the motor as a normalised value between -1.0 and 1.0
   */
  void setPower(double percentage);
  ServoMotor& operator=(int percentage);

  /** Read the current speed of the motor
   *
   * @return Current speed of motor
   */
  double getPower();

  // ContServo type exclusive functions
  void servoSetMaxSpeed(float max_speed_);
  void servoSetSpeed(float speed_);
  float servoRead(void);
  float servoGetMaxSpeed(void);
  void servoSetPeriod(int period);

 protected:
  bool m_inverted;
  double m_limit;
  ContServo m_servo;
};
