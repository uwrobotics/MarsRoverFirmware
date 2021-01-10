#pragma once

#ifndef PWM_IN_DEFAULT_NUM_SAMPLES_TO_AVERAGE
#define PWM_IN_DEFAULT_NUM_SAMPLES_TO_AVERAGE 12
#endif

#include "mbed.h"

/** PwmIn class to read PWM inputs
 *
 * Uses InterruptIn to measure the changes on the input
 * and record the time they occur
 *
 * @note uses InterruptIn, so not available on p19/p20
 */
class PwmIn {
 public:
  /** Create a PwmIn with a specified number of pulses to average
   *
   * @param pwmSense           The pwm input pin (must support InterruptIn)
   * @param numSamplesToAverage The number of PWM measurements to sum before averaging
   */
  PwmIn(PinName pwmSense, int numSamplesToAverage = PWM_IN_DEFAULT_NUM_SAMPLES_TO_AVERAGE);

  ~PwmIn();

  /** Read the current period
   *
   * @returns the period in seconds
   */
  std::chrono::duration<float> period();

  /** Read the average period
   *
   * @returns the average period in seconds
   */
  std::chrono::duration<float> avgPeriod();

  /** Read the current pulse width
   *
   * @returns the pulsewidth in seconds
   */
  std::chrono::duration<float> pulseWidth();

  /** Read the average pulse width
   *
   * @returns the average pulsewidth in seconds
   */
  std::chrono::duration<float> avgPulseWidth();

  /** Read the current duty cycle
   *
   * @returns the duty cycle as a percentage, represented between 0.0-1.0
   */
  float dutyCycle();

  /** Read the average duty cycle
   *
   * @returns the average duty cycle as a percentage, represented between 0.0-1.0
   */
  float avgDutyCycle();

  /** Read the average duty cycle velocity
   *
   * @returns the average duty cycle velocity as a 0.0-1.0 percentage/second
   */
  float avgDutyCycleVelocity();

 protected:
  InterruptIn m_pwmSense;
  Timer m_timer;

  std::chrono::duration<float> m_pulseWidth, m_period, m_avgPulseWidth, m_avgPeriod;
  float m_avgDutyCycle, m_prevAvgDutyCycle, m_avgDutyCycleVelocity;

  int m_sampleCount;
  int m_numSamplesToAverage;

  std::chrono::duration<float>* p_pulseWidthSamples;
  std::chrono::duration<float>* p_periodSamples;

  std::chrono::duration<float> m_pulseWidthSampleSum;
  std::chrono::duration<float> m_periodSampleSum;

  void rise();
  void fall();
  std::chrono::duration<float> movingAvg(std::chrono::duration<float>* p_samples,
                                         std::chrono::duration<float>* p_sampleSum,
                                         std::chrono::duration<float> newSample, int newIndex);
};
