/**
 * This PID library was inspired by http://brettbeauregard.com/blog/2011/04/improving-the-beginners-pid-introduction/
 * Terminology:
 *  Plant: What you are trying to control
 *  Process Variable(PV): Measured value of plant. Sensor reading.
 *  Setpoint: Desired state of the plant
 *  Deadzone: Value below which error is considered 0
 *  Gain: Scalar seen in the state space representation of a PID controller
 *  Path: A path on the state space representation of a PID controller
 * Features:
 *  Thread-safety: This PID library is thread safe.
 *                 WARNING: Mutexes wait for lock indefinetly, since deadlock is currently impossible
 *                          If modifying the code, ensure deadlock remains impossible
 *  Anti-Windup: See this video: https://www.youtube.com/watch?v=NVLXCwc8HzM&t=571s&ab_channel=MATLAB
 *               See also Brett Beauregard blog:
 *               http://brettbeauregard.com/blog/2011/04/improving-the-beginner%e2%80%99s-pid-reset-windup/
 * Anti-Derivative-Kickback: Avoid jerkiness, by differentiating on PV. See Brett Beauregard blog:
 *                           http://brettbeauregard.com/blog/2011/04/improving-the-beginner%e2%80%99s-pid-derivative-kick/
 * On-The-Fly Tuning: Allows gains to be changed with reduced joltiness. Done by multiplying the gain into the error
 *                    accumulator:
 * http://brettbeauregard.com/blog/2011/04/improving-the-beginner%e2%80%99s-pid-tuning-changes/
 *
 */
#pragma once

namespace PID {
typedef struct Config {
  float proportionalGain, integralGain, derivativeGain;
  float lowerBound, upperBound;
  float deadzone;
  bool antiKickback = true;
  bool antiWindup   = true;
} Config;
class PID {
 public:
  PID(const Config &config);

  void updateProportionalGain(float p);
  void updateIntegralGain(float i);
  void updateDerivativeGain(float d);
  void updateDeadzone(float deadzone);

  float reportProportionalGain() const;
  float reportIntegralGain() const;
  float reportDerivativeGain() const;
  float reportDeadzone() const;

  void reset();
  float compute(float setPoint, float processVariable, float ff = 0);  // takes ~15us to run

 private:
  mutable Mutex m_mutex;
  Timer m_timer;
  float m_PGain, m_IGain, m_DGain;
  const float m_lowerBound, m_upperBound;
  float m_deadzone;
  float m_IAccumulator{0};
  float m_pastError{0}, m_pastPV{0};
  const bool m_antiKickback, m_antiWindup;
  float computePPath(float error);
  float computeDPathOnError(float error, float dt);
  float computeDPathOnPV(float PV, float dt);
};
}  // namespace PID
