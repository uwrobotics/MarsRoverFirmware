// ADD COMMENT EXPLAINIG TERMINOLGY AND LINK TO TEMPLATE FOR THIS CODE
#include <atomic>
#include "mbed.h"

class PID {
 public:
  PID(uint32_t proportionalGain, uint32_t intregralGain, uint32_t derivativeGain, int32_t lowerBound,
      int32_t upperBound, float deadzone, bool useAntiDerivativeKickback = true);

  // WARNING: THESE ARE CALLED IN THE CANRX THREAD
  mbed_error_status_t updateProportionalGain(const uint32_t p);
  mbed_error_status_t updateIntegralGain(const uint32_t i);
  mbed_error_status_t updateDerivativeGain(const uint32_t d);
  mbed_error_status_t updateDeadzone(const float deadzone);

  // WARNING: THESE ARE CALLED IN CANRX THREAD
  uint32_t reportProportionalGain() const;
  uint32_t reportIntegralGain() const;
  uint32_t reportDerivativeGain() const;
  float reportDeadzone() const;

  // WARNING: THESE ARE CALLED IN CANRX THREAD
  void reset();
  // WARNING: COMPUTE IS CALLED IN MAIN THREAD
  float compute(float setPoint, float processVariable);

 private:
  // Watch out for deadlock if editing this code
  Mutex m_timerMutex;
  // TIMER SHOULD BE ATOMIC/MUTEX PROTECTED
  Timer m_timer;
  std::atomic<uint32_t> m_PGain, m_IGain, m_DGain;
  const int32_t m_lowerBound, m_upperBound;
  // NO ATOMIC INCREMENT FOR FLOATS?????????/
  std::atomic<float> m_deadzone;
  mutable std::atomic<float> m_IPath;
  std::atomic<float> m_pastError, m_pastPV;
  const bool m_antiKickback;
  float computePPath(float error) const;
  float computeIPath(float error, int64_t dt) const;
  float computeDPathOnError(float error, int64_t dt) const;
  float computeDPathOnPV(float PV, int64_t dt) const;
};
