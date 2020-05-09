#include "PwmIn.h"

PwmIn::PwmIn(PinName pwmSense, int numSamplesToAverage)
    : m_pwmSense(pwmSense), m_numSamplesToAverage(numSamplesToAverage) {
  m_pwmSense.rise(callback(this, &PwmIn::rise));
  m_pwmSense.fall(callback(this, &PwmIn::fall));

  m_period = 0.0;
  m_pulseWidth = 0.0;
  m_periodSampleSum = 0.0;
  m_pulseWidthSampleSum = 0.0;
  m_sampleCount = 0;
  m_avgDutyCycle = 0;
  m_prevAvgDutyCycle = 0;
  m_avgDutyCycleVelocity = 0;

  p_periodSamples = new float[m_numSamplesToAverage]();
  p_pulseWidthSamples = new float[m_numSamplesToAverage]();

  m_timer.start();
}

PwmIn::~PwmIn() {
  delete[] p_pulseWidthSamples;
  delete[] p_periodSamples;
}

float PwmIn::period() {
  return m_period;
}

float PwmIn::avgPeriod() {
  return m_avgPeriod;
}

float PwmIn::pulseWidth() {
  return m_pulseWidth;
}

float PwmIn::avgPulseWidth() {
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
  m_period = m_timer.read();
  m_timer.reset();

  m_avgPeriod = PwmIn::movingAvg(p_periodSamples, &m_periodSampleSum, m_period, m_sampleCount);
}

void PwmIn::fall() {
  m_pulseWidth = m_timer.read();

  m_avgPulseWidth = PwmIn::movingAvg(p_pulseWidthSamples, &m_pulseWidthSampleSum, m_pulseWidth, m_sampleCount);
  m_avgDutyCycle = m_avgPulseWidth / m_avgPeriod;
  m_avgDutyCycleVelocity = (m_avgDutyCycle - m_prevAvgDutyCycle) / m_avgPeriod;
  m_prevAvgDutyCycle = m_avgDutyCycle;

  m_sampleCount++;

  if (m_sampleCount >= m_numSamplesToAverage) {
    m_sampleCount = 0;
  }
}

float PwmIn::movingAvg(float* p_samples, float* p_sampleSum, float newSample, int newIndex) {
  *p_sampleSum -= p_samples[newIndex];
  p_samples[newIndex] = newSample;
  *p_sampleSum += p_samples[newIndex];

  return *p_sampleSum / (float)m_numSamplesToAverage;
}
