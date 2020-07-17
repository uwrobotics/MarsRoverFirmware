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
   * @param min_pulsewidth  Pwm pulse width for minimum speed
   * @param max_pulsewidth  Pwm pulse width for maximum speed
   * @param limit     Maximum speed magnitude
   */
  ServoMotor(PinName pwm, bool inverted = false, std::chrono::duration<double> min_pulsewidth = 1ms, std::chrono::duration<double> max_pulsewidth = 2ms,
             double limit = 1.0);

  ServoMotor(Motor::t_motorConfig motorConfig);

  /** Set the speed of the motor
   *
   * @param percentage The speed of the motor as a normalised value between -1.0 and 1.0
   */
  void setPower(double percentage);
  ServoMotor& operator=(double percentage);

  /** Read the current speed of the motor
   *
   * @return Current speed of motor
   */
  double getPower();

  // ContServo type exclusive functions
  void servoSetMaxSpeed(double max_speed_);
  void servoSetSpeed(double speed_);
  float servoRead(void);
  float servoGetMaxSpeed(void);
  void servoSetPeriod(std::chrono::duration<double> period);

 protected:
  bool m_inverted;
  double m_limit;
  ContServo m_servo;
};
