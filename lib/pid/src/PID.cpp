#include "PID.h"
#include <algorithm>

PID::PID(uint32_t proportionalGain, uint32_t intregralGain, uint32_t derivativeGain, int32_t lowerBound,
         int32_t upperBound, float deadzone, bool antiKickback)
    : m_PGain(proportionalGain),
      m_IGain(intregralGain),
      m_DGain(derivativeGain),
      m_lowerBound(lowerBound),
      m_upperBound(upperBound),
      m_deadzone(deadzone),
      m_antiKickback(antiKickback),
      m_IPath(0),
      m_pastError(0),
      m_pastPV(0) {}

mbed_error_status_t PID::updateProportionalGain(uint32_t p) {
  m_PGain.store(p);
  return MBED_SUCCESS;
}

mbed_error_status_t PID::updateIntegralGain(uint32_t i) {
  m_IGain.store(i);
  return MBED_SUCCESS;
}

mbed_error_status_t PID::updateDerivativeGain(uint32_t d) {
  m_DGain.store(d);
  return MBED_SUCCESS;
}

mbed_error_status_t PID::updateDeadzone(float deadzone) {
  m_deadzone.store(deadzone);
  return MBED_SUCCESS;
}

uint32_t PID::reportProportionalGain() const {
  return m_PGain.load();
}

uint32_t PID::reportIntegralGain() const {
  return m_IGain.load();
}

uint32_t PID::reportDerivativeGain() const {
  return m_DGain.load();
}

float PID::reportDeadzone() const {
  return m_deadzone.load();
}

void PID::reset() {
  m_IPath.store(0);
  m_pastError.store(0);
  m_pastPV.store(0);
  m_timerMutex.lock();
  m_timer.stop();  // avoid spurious start after reset
  m_timer.reset();
  m_timerMutex.unlock()
}

float PID::computePPath(float error) const {
  return error * m_PGain.load();
}

float PID::computeIPath(float error, int64_t dt) const {
  // figure out this fetch add what 2nd arg to pass?
  m_IPath.fetch_add(error * dt * m_IGain.load());
  m_IPath.store(std::clamp(m_IPath.load(), static_cast<float>(m_lowerBound), static_cast<float>(m_upperBound)));
  return m_IPath.load();
}

float PID::computeDPathOnError(float error, int64_t dt) const {
  float derivativePath = 0;
  if (dt != 0) {
    derivativePath = m_DGain.load() * (error - m_pastError.load()) / dt;
  }
  return derivativePath;
}

float PID::computeDPathOnPV(float processVariable, int64_t dt) const {
  float derivativePath = 0;
  if (dt != 0) {
    derivativePath = m_DGain.load() * (processVariable - m_pastPV.load()) / dt;
  }
  return derivativePath;
}

float PID::compute(float setPoint, float processVariable) {
  float error = setPoint - processVariable;
  if (std::abs(error) < m_deadzone.load()) {
    error = 0;
  }

  m_timerMutex.lock();
  m_timer.stop();
  int64_t dt = m_timer.elapsed_time().count();
  m_timerMutex.unlock();

  float paths = computePPath(error);
  paths += computeIPath(error, dt);
  paths += m_antiKickback ? computeDPathOnPV(error, dt) : computeDPathOnError(processVariable, dt);
  paths = std::clamp(paths, static_cast<float>(m_lowerBound), static_cast<float>(m_upperBound));

  m_pastError.store(error);
  m_pastPV.store(processVariable);
  m_timerMutex.lock();
  m_timer.start();
  m_timerMutex.unlock();
  return paths;
}
