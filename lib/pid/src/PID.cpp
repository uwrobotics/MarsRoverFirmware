#include "PID.h"

#include <algorithm>
#include <mutex>

using namespace PID;

PID::PID::PID(const Config &config)
    : m_PGain(config.proportionalGain),
      m_IGain(config.integralGain),
      m_DGain(config.derivativeGain),
      m_lowerBound(config.lowerBound),
      m_upperBound(config.upperBound),
      m_deadzone(config.deadzone),
      m_antiKickback(config.antiKickback),
      m_antiWindup(config.antiWindup) {}

void PID::PID::updateProportionalGain(float p) {
  std::scoped_lock<Mutex> lock(m_mutex);
  m_PGain = p;
}

void PID::PID::updateIntegralGain(float i) {
  std::scoped_lock<Mutex> lock(m_mutex);
  m_IGain = i;
}

void PID::PID::updateDerivativeGain(float d) {
  std::scoped_lock<Mutex> lock(m_mutex);
  m_DGain = d;
}

void PID::PID::updateDeadzone(float deadzone) {
  std::scoped_lock<Mutex> lock(m_mutex);
  m_deadzone = deadzone;
}

float PID::PID::reportProportionalGain() const {
  std::scoped_lock<Mutex> lock(m_mutex);
  return m_PGain;
}

float PID::PID::reportIntegralGain() const {
  std::scoped_lock<Mutex> lock(m_mutex);
  return m_IGain;
}

float PID::PID::reportDerivativeGain() const {
  std::scoped_lock<Mutex> lock(m_mutex);
  return m_DGain;
}

float PID::PID::reportDeadzone() const {
  std::scoped_lock<Mutex> lock(m_mutex);
  return m_deadzone;
}

void PID::PID::reset() {
  std::scoped_lock<Mutex> lock(m_mutex);
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

float PID::PID::computeDPathOnError(float error, float dt) {
  // no mutex lock needed since inside compute() only
  float derivativePath = 0;
  if (dt != 0) {
    derivativePath = m_DGain * (error - m_pastError) / dt;  // todo this keeps dividing by a small number
  }
  return derivativePath;
}

float PID::PID::computeDPathOnPV(float processVariable, float dt) {
  // no mutex lock needed since inside compute() only
  float derivativePath = 0;
  if (dt != 0) {
    derivativePath = m_DGain * (processVariable - m_pastPV) / dt;
  }
  return -derivativePath;  // since d_error/dt = -d_pv/dt
}

float PID::PID::compute(float setPoint, float processVariable, float ff) {
  std::scoped_lock<Mutex> lock(m_mutex);
  float error = setPoint - processVariable;
  if (std::abs(error) < m_deadzone) {
    error = 0;
  }

  m_timer.stop();
  float dt    = chrono::duration_cast<chrono::duration<float>>(m_timer.elapsed_time()).count();  // seconds
  float paths = ff;
  paths += computePPath(error);
  m_IAccumulator += error * dt * m_IGain;
  paths += m_IAccumulator;
  paths += m_antiKickback ? computeDPathOnPV(processVariable, dt) : computeDPathOnError(error, dt);

  if (m_antiWindup) {
    if (paths > m_upperBound) {
      m_IAccumulator -= paths - m_upperBound;
      paths = m_upperBound;
    } else if (paths < m_lowerBound) {
      m_IAccumulator += m_lowerBound - paths;
      paths = m_lowerBound;
    }
  } else {
    paths = std::clamp(paths, m_lowerBound, m_upperBound);  // clamp without affecting accumulator
  }

  m_pastError = error;
  m_pastPV    = processVariable;
  m_timer.reset();
  m_timer.start();
  return paths;
}
