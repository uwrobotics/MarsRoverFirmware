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
 *               See also Brett Beauregard blog
 *  Anti-Derivative-Kickback: Avoid jerkiness, by differentiating on PV. See Brett Beauregard blog
 *
 */
#include <atomic>
#include "mbed.h"

// TODO: WRITE A TEST APP AND FIX COMMENTS


class PID {
 public:
  PID(uint32_t proportionalGain, uint32_t intregralGain, uint32_t derivativeGain, int32_t lowerBound,
      int32_t upperBound, float deadzone, bool useAntiDerivativeKickback = true);

  // WARNING: THESE ARE CALLED IN THE CANRX THREAD
  void updateProportionalGain(uint32_t p);
  void updateIntegralGain(uint32_t i);
  void updateDerivativeGain(uint32_t d);
  void updateDeadzone(float deadzone);

  // WARNING: THESE ARE CALLED IN CANRX THREAD
  uint32_t reportProportionalGain() const;
  uint32_t reportIntegralGain() const;
  uint32_t reportDerivativeGain() const;
  float reportDeadzone() const;

  // WARNING: THESE ARE CALLED IN CANRX THREAD
  void reset();
  // WARNING: COMPUTE IS CALLED IN MAIN THREAD
  float compute(float setPoint, float processVariable) const;

 private:
  Mutex m_mutex;
  Timer m_timer;
  uint32_t m_PGain, m_IGain, m_DGain;
  const int32_t m_lowerBound, m_upperBound;
  float m_deadzone;
  mutable float m_IPath;
  float m_pastError, m_pastPV;
  const bool m_antiKickback;
  float computePPath(float error) const;
  float computeIPath(float error, int64_t dt) const;
  float computeDPathOnError(float error, int64_t dt) const;
  float computeDPathOnPV(float PV, int64_t dt) const;
};
