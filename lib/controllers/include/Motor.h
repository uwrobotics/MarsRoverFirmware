#pragma once

#include <bits/stdint-uintn.h>
constexpr unsigned uint16_t MOTOR_DEFAULT_FREQUENCY = 244;

#include "mbed.h"

// Interface to control a standard DC motor with an H-bridge using a PwmOut and 2 DigitalOuts

class Motor {
 public:
  typedef struct motorConfig {
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
  Motor(PinName pwm, PinName dir, bool inverted = false, int freqInHz = MOTOR_DEFAULT_FREQUENCY_HZ, float limit = 1.0);

  Motor(t_motorConfig motorConfig);

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

 protected:
  PwmOut m_pwm;
  DigitalOut m_dir;
  bool m_inverted;
  float m_limit;
};
