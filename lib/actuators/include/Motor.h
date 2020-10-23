#pragma once

#include "Actuator.h"
#include "mbed.h"

class Motor : public Actuator {
 protected:
  static constexpr int MOTOR_DEFAULT_FREQUENCY_HZ = 1000;  // TODO

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
   * @param freqInHz  Output PWM frequency, default 1kHz (TODO)
   * @param inverted  If true, then forward speed will set dir to 0 instead of 1, otherwise inverse
   * @param limit     Maximum speed magnitude
   */
  Motor(PinName pwm, PinName dir, bool inverted = false, int freqInHz = MOTOR_DEFAULT_FREQUENCY_HZ, float limit = 1.0);

  Motor(t_motorConfig motorConfig);

  /** Set the speed of the motor
   *
   * @param dutyCycle The speed of the motor as a normalised value between -1.0 and 1.0
   */
  void setValue(float dutyCycle);
  Actuator& operator=(float dutyCycle);

  /** Read the current speed of the motor
   *
   * @return Current speed of motor
   */
  float getValue();

 protected:
  PwmOut m_pwm;
  DigitalOut m_dir;
  bool m_inverted;
  float m_limit;
};
