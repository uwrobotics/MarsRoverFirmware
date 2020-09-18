#include "PwmIn.h"

PwmIn::PwmIn(PinName pwmSense, int numSamplesToAverage)
    : m_pwmSense(pwmSense), m_numSamplesToAverage(numSamplesToAverage) {
  m_pwmSense.rise(callback(this, &PwmIn::rise));
  m_pwmSense.fall(callback(this, &PwmIn::fall));

  m_period               = 0s;
  m_pulseWidth           = 0s;
  m_periodSampleSum      = 0s;
  m_pulseWidthSampleSum  = 0s;
  m_sampleCount          = 0;
  m_avgDutyCycle         = 0;
  m_prevAvgDutyCycle     = 0;
  m_avgDutyCycleVelocity = 0;

  p_periodSamples     = new std::chrono::duration<float>[m_numSamplesToAverage]();
  p_pulseWidthSamples = new std::chrono::duration<float>[m_numSamplesToAverage]();

  m_timer.start();
}

PwmIn::~PwmIn() {
  delete[] p_pulseWidthSamples;
  delete[] p_periodSamples;
}

std::chrono::duration<float> PwmIn::period() {
  return m_period;
}

std::chrono::duration<float> PwmIn::avgPeriod() {
  return m_avgPeriod;
}

std::chrono::duration<float> PwmIn::pulseWidth() {
  return m_pulseWidth;
}

std::chrono::duration<float> PwmIn::avgPulseWidth() {
  return m_avgPulseWidth;
}

float PwmIn::dutyCycle() {
  return m_pulseWidth / m_period;
}

float PwmIn::avgDutyCycle() {
  return m_avgDutyCycle;
}

float PwmIn::avgDutyCycleVelocity() {
  return m_avgDutyCycleVelocity;
}

void PwmIn::rise() {
  m_period = m_timer.elapsed_time();
  m_timer.reset();

  m_avgPeriod = PwmIn::movingAvg(p_periodSamples, &m_periodSampleSum, m_period, m_sampleCount);
}

void PwmIn::fall() {
  m_pulseWidth = m_timer.elapsed_time();

  m_avgPulseWidth        = PwmIn::movingAvg(p_pulseWidthSamples, &m_pulseWidthSampleSum, m_pulseWidth, m_sampleCount);
  m_avgDutyCycle         = m_avgPulseWidth / m_avgPeriod;
  m_avgDutyCycleVelocity = (m_avgDutyCycle - m_prevAvgDutyCycle) / m_avgPeriod.count();
  m_prevAvgDutyCycle     = m_avgDutyCycle;

  m_sampleCount++;

  if (m_sampleCount >= m_numSamplesToAverage) {
    m_sampleCount = 0;
  }
}

std::chrono::duration<float> PwmIn::movingAvg(std::chrono::duration<float>* p_samples,
                                              std::chrono::duration<float>* p_sampleSum,
                                              std::chrono::duration<float> newSample, int newIndex) {
  *p_sampleSum -= p_samples[newIndex];
  p_samples[newIndex] = newSample;
  *p_sampleSum += p_samples[newIndex];

  return *p_sampleSum / m_numSamplesToAverage;
}
