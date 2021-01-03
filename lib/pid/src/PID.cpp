#include "PID.h"

#include <algorithm>
#include <mutex>

using namespace PID;

PID::PID::PID(float proportionalGain, float intregralGain, float derivativeGain, float lowerBound, float upperBound,
              float deadzone, bool antiKickback)
    : m_PGain(proportionalGain),
      m_IGain(intregralGain),
      m_DGain(derivativeGain),
      m_lowerBound(lowerBound),
      m_upperBound(upperBound),
      m_deadzone(deadzone),
      m_antiKickback(antiKickback) {}

PID::PID::PID(const Config &config)
    : PID(config.proportionalGain, config.integralGain, config.derivativeGain, config.lowerBound, config.upperBound,
          config.deadzone, config.antiKickback) {}

void PID::PID::updateProportionalGain(float p) {
  std::lock_guard<Mutex> lock(m_mutex);
  m_PGain = p;
}

void PID::PID::updateIntegralGain(float i) {
  std::lock_guard<Mutex> lock(m_mutex);
  m_IGain = i;
}

void PID::PID::updateDerivativeGain(float d) {
  std::lock_guard<Mutex> lock(m_mutex);
  m_DGain = d;
}

void PID::PID::updateDeadzone(float deadzone) {
  std::lock_guard<Mutex> lock(m_mutex);
  m_deadzone = deadzone;
}

float PID::PID::reportProportionalGain() const {
  std::lock_guard<Mutex> lock(m_mutex);
  return m_PGain;
}

float PID::PID::reportIntegralGain() const {
  std::lock_guard<Mutex> lock(m_mutex);
  return m_IGain;
}

float PID::PID::reportDerivativeGain() const {
  std::lock_guard<Mutex> lock(m_mutex);
  return m_DGain;
}

float PID::PID::reportDeadzone() const {
  std::lock_guard<Mutex> lock(m_mutex);
  return m_deadzone;
}

void PID::PID::reset() {
  std::lock_guard<Mutex> lock(m_mutex);
  m_IAccumulator = 0;
  m_pastError    = 0;
  m_pastPV       = 0;
  m_timer.stop();
  m_timer.reset();
}

float PID::PID::computePPath(float error) {
  // no mutex lock needed since inside compute() only
  return error * m_PGain;
}

float PID::PID::computeIPath(float error, int64_t dt) {
  // no mutex lock needed since inside compute() only
  m_IAccumulator += error * dt * m_IGain;
  m_IAccumulator = std::clamp(m_IAccumulator, static_cast<float>(m_lowerBound), static_cast<float>(m_upperBound));
  return m_IAccumulator;
}

float PID::PID::computeDPathOnError(float error, int64_t dt) {
  // no mutex lock needed since inside compute() only
  float derivativePath = 0;
  if (dt != 0) {
    derivativePath = m_DGain * (error - m_pastError) / dt;
  }
  return derivativePath;
}

float PID::PID::computeDPathOnPV(float processVariable, int64_t dt) {
  // no mutex lock needed since inside compute() only
  float derivativePath = 0;
  if (dt != 0) {
    derivativePath = m_DGain * (processVariable - m_pastPV) / dt;
  }
  return -derivativePath;
}

float PID::PID::compute(float setPoint, float processVariable) {
  std::lock_guard<Mutex> lock(m_mutex);
  float error = setPoint - processVariable;
  if (std::abs(error) < m_deadzone) {
    error = 0;
  }

  m_timer.stop();
  float dt    = chrono::duration_cast<chrono::duration<float>>(m_timer.elapsed_time()).count();  // seconds
  float paths = computePPath(error);
  paths += computeIPath(error, dt);
  paths += m_antiKickback ? computeDPathOnPV(processVariable, dt) : computeDPathOnError(error, dt);
  paths = std::clamp(paths, static_cast<float>(m_lowerBound), static_cast<float>(m_upperBound));

  m_pastError = error;
  m_pastPV    = processVariable;
  m_timer.reset();
  m_timer.start();
  return paths;
}
