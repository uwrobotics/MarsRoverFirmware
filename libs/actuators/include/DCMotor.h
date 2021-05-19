#pragma once

#include "Actuator.h"
#include "mbed.h"

namespace Actuator {
class DCMotor final : public Actuator {
 public:
  typedef struct Config {
    PinName pwmPin;
    PinName dirPin;
    bool inverted;
    int freqInHz;
    float limit;
  } Config;
  /** Create a motor control interface
   *
   * @param pwm       A PwmOut pin, driving the H-bridge enable line to control the speed
   * @param dir       A DigitalOut, set high when the motor should go forward, low when backwards
   * @param freqInHz  Output PWM frequency, default 20kHz
   * @param inverted  If true, then forward speed will set dir to 0 instead of 1, otherwise inverse
   * @param limit     Maximum speed magnitude
   */
  DCMotor(PinName pwm, PinName dir, bool inverted = false, int freqInHz = MOTOR_DEFAULT_FREQUENCY_HZ,
          float limit = 1.0);

  DCMotor(const Config &config);

  /** Set the speed of the motor
   *
   * @param dutyCycle The speed of the motor as a normalised value between -1.0 and 1.0
   */
  void setValue(float dutyCycle) override;

 private:
  static constexpr int MOTOR_DEFAULT_FREQUENCY_HZ = 20000;  // TODO: Need to test if 20kHz works

  PwmOut m_pwm;
  DigitalOut m_dir;
  bool m_inverted;
  float m_limit;
};
}  // namespace Actuator
