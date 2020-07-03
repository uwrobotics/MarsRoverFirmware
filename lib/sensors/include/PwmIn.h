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
   * @returns the period in microseconds
   */
  std::chrono::microseconds period();

  /** Read the average period
   *
   * @returns the average period in microseconds
   */
  std::chrono::microseconds avgPeriod();

  /** Read the current pulse width
   *
   * @returns the pulsewidth in microseconds
   */
  std::chrono::microseconds pulseWidth();

  /** Read the average pulse width
   *
   * @returns the average pulsewidth in microseconds
   */
  std::chrono::microseconds avgPulseWidth();

  /** Read the current duty cycle
   *
   * @returns the duty cycle as a percentage, represented between 0.0-1.0
   */
  double dutyCycle();

  /** Read the average duty cycle
   *
   * @returns the average duty cycle as a percentage, represented between 0.0-1.0
   */
  double avgDutyCycle();

  /** Read the average duty cycle velocity
   *
   * @returns the average duty cycle velocity as a 0.0-1.0 percentage/second
   */
  double avgDutyCycleVelocity();

 protected:
  InterruptIn m_pwmSense;
  Timer m_timer;

  std::chrono::microseconds m_pulseWidth, m_period, m_avgPulseWidth, m_avgPeriod;
  double m_avgDutyCycle, m_prevAvgDutyCycle, m_avgDutyCycleVelocity;

  int m_sampleCount;
  int m_numSamplesToAverage;

  std::chrono::microseconds* p_pulseWidthSamples;
  std::chrono::microseconds* p_periodSamples;

  std::chrono::microseconds m_pulseWidthSampleSum;
  std::chrono::microseconds m_periodSampleSum;

  void rise();
  void fall();
  std::chrono::microseconds movingAvg(std::chrono::microseconds* p_samples, std::chrono::microseconds* p_sampleSum, std::chrono::microseconds newSample, int newIndex);
};
