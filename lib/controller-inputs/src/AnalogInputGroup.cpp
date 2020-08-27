#include "AnalogInputGroup.h"

AnalogInputGroup::AnalogInputGroup(AnalogBusIn& inputs, uint8_t numPins, AveragingAlgoType averagingAlgoType,
                                   uint8_t numSampleToAverage, float decayFactor)
    : m_inputs(inputs),
      m_numPins(numPins),
      m_decayFactor(decayFactor),
      m_num_samples_to_average(numSampleToAverage),
      m_averagingAlgoType(averagingAlgoType) {}

uint8_t AnalogInputGroup::getNumInputs() const {
  return m_numPins;
}

void AnalogInputGroup::read() {
  switch (m_averagingAlgoType) {
    case AveragingAlgoType::SMA:
      readSMA();
      break;

    case AveragingAlgoType::EWMA:
      readEWMA();
      break;

    default:
      readSMA();
      break;
  }
}

void AnalogInputGroup::readSMA() {
  static uint16_t values[MAX_INPUTS_NUM] = {0};

  // read the values
  m_inputs.read_u16(values);

  // apply simple moving average
  for (uint16_t i = 0; i < m_numPins; i++) {
    uint16_t curr_idx = m_values_idx[i];

    m_values_sum[i] -= m_values_SMA[i][curr_idx];
    m_values_SMA[i][curr_idx] = values[i];
    m_values_sum[i] += m_values_SMA[i][curr_idx];

    curr_idx++;
    // wrap around the index
    if (curr_idx > m_num_samples_to_average) curr_idx = 0;

    // we also record how many sample we have read so far, and cap it at to the value of
    // m_num_samples_to_average
    if (m_num_samples < m_num_samples_to_average) m_num_samples++;

    m_values_idx[i] = curr_idx;
  }
}

void AnalogInputGroup::readEWMA() {
  static uint16_t values[MAX_INPUTS_NUM] = {0};

  // read the values
  m_inputs.read_u16(values);

  // apply exponentially weighted moving average
  for (uint16_t i = 0; i < m_numPins; i++) {
    m_values_EWMA[i] = m_decayFactor * values[i] + (1.0f - m_decayFactor) * m_values_EWMA[i];
  }
}

void AnalogInputGroup::getValues(float* values) {
  switch (m_averagingAlgoType) {
    case AveragingAlgoType::SMA:
      getValuesSMA(values);
      break;

    case AveragingAlgoType::EWMA:
      getValuesEWMA(values);
      break;

    default:
      getValuesSMA(values);
      break;
  }
}

void AnalogInputGroup::getValuesSMA(float* values) {
  for (uint16_t i = 0; i < m_numPins; i++) {
    uint16_t u16_val = m_values_sum[i] / m_num_samples;

    // normalize from 0x0 - 0xFFFF to 0.0 - 1.0
    values[i] = float(u16_val) / float(0xFFFF);  // no need to * 1.0;
  }

  reset();
}

void AnalogInputGroup::getValuesEWMA(float* values) {
  for (uint16_t i = 0; i < m_numPins; i++) {
    uint16_t u16_val = m_values_EWMA[i];

    // normalize from 0x0 - 0xFFFF to 0.0 - 1.0
    values[i] = float(u16_val) / float(0xFFFF);  // no need to * 1.0;
  }

  reset();
}

void AnalogInputGroup::getValues(uint16_t* values) {
  switch (m_averagingAlgoType) {
    case AveragingAlgoType::SMA:
      getValuesSMA(values);
      break;

    case AveragingAlgoType::EWMA:
      getValuesEWMA(values);
      break;

    default:
      getValuesSMA(values);
      break;
  }
}

void AnalogInputGroup::getValuesEWMA(uint16_t* values) {
  for (uint16_t i = 0; i < m_numPins; i++) {
    values[i] = m_values_sum[i] / m_num_samples;
  }

  reset();
}

void AnalogInputGroup::getValuesSMA(uint16_t* values) {
  for (uint16_t i = 0; i < m_numPins; i++) {
    values[i] = m_values_EWMA[i];
  }

  reset();
}

void AnalogInputGroup::reset() {
  m_num_samples = 0;

  for (uint16_t i = 0; i < m_numPins; i++) {
    m_values_EWMA[i] = 0;
    m_values_idx[i]  = 0;
    m_values_sum[i]  = 0;

    for (uint8_t j = 0; j < m_num_samples_to_average; j++) {
      m_values_SMA[i][j] = 0;
    }
  }
}