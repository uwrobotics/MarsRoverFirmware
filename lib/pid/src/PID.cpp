#include "PID.h"
#include <algorithm>
#include <mutex>

using namespace PID;

PID::Pid::Pid(uint32_t proportionalGain, uint32_t intregralGain, uint32_t derivativeGain, int32_t lowerBound,
         int32_t upperBound, float deadzone, bool antiKickback)
    : m_PGain(proportionalGain),
      m_IGain(intregralGain),
      m_DGain(derivativeGain),
      m_lowerBound(lowerBound),
      m_upperBound(upperBound),
      m_deadzone(deadzone),
      m_IPath(0),
      m_pastError(0),
      m_pastPV(0), 
      m_antiKickback(antiKickback) {}

void PID::Pid::updateProportionalGain(uint32_t p) {
  std::lock_guard<Mutex> lock(m_mutex);
  m_PGain = p;
}

void PID::Pid::updateIntegralGain(uint32_t i) {
  std::lock_guard<Mutex> lock(m_mutex);
  m_IGain = i;
}

void PID::Pid::updateDerivativeGain(uint32_t d) {
  std::lock_guard<Mutex> lock(m_mutex);
  m_DGain = d;
}

void PID::Pid::updateDeadzone(float deadzone) {
  std::lock_guard<Mutex> lock(m_mutex);
  m_deadzone = deadzone;
}

uint32_t PID::Pid::reportProportionalGain() const {
  std::lock_guard<Mutex> lock(m_mutex);
  return m_PGain;
}

uint32_t PID::Pid::reportIntegralGain() const {
  std::lock_guard<Mutex> lock(m_mutex);
  return m_IGain;
}

uint32_t PID::Pid::reportDerivativeGain() const {
  std::lock_guard<Mutex> lock(m_mutex);
  return m_DGain;
}

float PID::Pid::reportDeadzone() const {
  std::lock_guard<Mutex> lock(m_mutex);
  return m_deadzone;
}

void PID::Pid::reset() {
  std::lock_guard<Mutex> lock(m_mutex);
  m_IPath = 0;
  m_pastError = 0;
  m_pastPV = 0;
  m_timer.stop();
  m_timer.reset();
}

float PID::Pid::computePPath(float error) {
  // no mutex lock needed since inside compute() only
  return error * m_PGain;
}

float PID::Pid::computeIPath(float error, int64_t dt) {
  // no mutex lock needed since inside compute() only
  m_IPath += error * dt * m_IGain;
  m_IPath = std::clamp(m_IPath, static_cast<float>(m_lowerBound), static_cast<float>(m_upperBound));
  return m_IPath;
}

float PID::Pid::computeDPathOnError(float error, int64_t dt)  {
  // no mutex lock needed since inside compute() only
  float derivativePath = 0;
  if (dt != 0) {
    derivativePath = m_DGain * (error - m_pastError) / dt;
  }
  return derivativePath;
}

float PID::Pid::computeDPathOnPV(float processVariable, int64_t dt) {
  // no mutex lock needed since inside compute() only
  float derivativePath = 0;
  if (dt != 0) {
    derivativePath = m_DGain * (processVariable - m_pastPV) / dt;
  }
  return derivativePath;
}

float PID::Pid::compute(float setPoint, float processVariable) {
  std::lock_guard<Mutex> lock(m_mutex);
  float error = setPoint - processVariable;
  if (std::abs(error) < m_deadzone) {
    error = 0;
  }

  m_timer.stop();
  float dt = chrono::duration_cast<chrono::duration<float>>(m_timer.elapsed_time()).count(); // seconds
  float paths = computePPath(error);
  paths += computeIPath(error, dt);
  paths += m_antiKickback ? computeDPathOnPV(error, dt) : computeDPathOnError(processVariable, dt);
  paths = std::clamp(paths, static_cast<float>(m_lowerBound), static_cast<float>(m_upperBound));

  m_pastError = error;
  m_pastPV = processVariable;
  m_timer.reset();
  m_timer.start();
  return paths;
}
