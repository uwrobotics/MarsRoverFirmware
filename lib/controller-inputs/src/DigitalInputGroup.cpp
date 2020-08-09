#include "DigitalInputGroup.h"

DigitalInputGroup::DigitalInputGroup(BusIn& inputs, uint8_t numPins, InputDebounceType debounceType,
                                     uint16_t integratingDebounceThres, std::chrono::microseconds delayDebouceTime)
    : m_inputs(inputs),
      m_numPins(numPins),
      m_integratingDebounceThres(integratingDebounceThres),
      m_delayDebounceTime(delayDebouceTime),
      m_debounceType(debounceType) {
  // To check constructor parameter at compile time involves using template, don't think that is appropriate here ...
  // So we kind of have to fail this at runtime...it would fail anyways later on if numPins is too large
  MBED_ASSERT(numPins <= MAX_INPUTS_NUM && numPins != 0);

  m_timer.start();
}

uint8_t DigitalInputGroup::getNumInputs() const {
  return m_numPins;
}

/* It might be a better architecture to separate out each debounce type into separate classes,
   but I think the complexity in this class right now is still manageable and it is unlikedly
   that we add new types of debouncing.
*/
void DigitalInputGroup::read() {
  switch (m_debounceType) {
    case InputDebounceType::NONE:
      readNoDebounce();
      break;

    case InputDebounceType::DELAY:
      readDelayDebounce();
      break;

    case InputDebounceType::INTERGRATING:
      readIntegratingDebounce();
      break;

    default:
      readNoDebounce();
      break;
  }
}

void DigitalInputGroup::readNoDebounce() {
  m_values = m_inputs.read();
}

void DigitalInputGroup::readDelayDebounce() {
  int reading = m_inputs.read();

  for (uint16_t i = 0; i < m_numPins; i++) {
    // if the input changed, due to noise or actual actions (press, touch, etc.):
    // Note if it is a noise, then the change would happen in 2x times (think of a spike, or a series of spikes)
    // (reading ^ m_curr_values) gives the bits that have changed, then shift to the first bit, and then check if it is
    // 0 or 1
    if (((reading ^ m_curr_values) >> i) & 0x1) {
      // reset the delay start time
      m_startTimes[i] = m_timer.elapsed_time();
      ;

      // set the delay started flag
      m_delayStarted |= (1 << i);

      // set the invalid bit since delay is not finished (it just started)
      m_invalidReads |= (1 << i);
    }

    // if "delay" has started, and we have "delayed" long enough
    if ((m_delayStarted & (1 << i)) && interval_passed(m_startTimes[i], m_timer.elapsed_time(), m_delayDebounceTime)) {
      // clear the invalid bit because delay has completed
      m_invalidReads &= ~(1 << i);

      // if the reading has remained same during the "delay" period, use it, and it is a valid read
      if (((reading & m_curr_values) >> i) & 0x1) {
        m_values |= (reading & (1 << i));

        // invalid bit is already cleared
      }
      // else, the read is invalid
      else {
        m_invalidReads |= (1 << i);
      }

      // clear the delay stated flag
      m_delayStarted &= ~(1 << i);
    }
  }

  // save the reading to be used when the next time this function is called
  m_curr_values = reading;
}

void DigitalInputGroup::readIntegratingDebounce() {
  int reading = m_inputs.read();

  // unlike the delay debouncing, we don't necessarily need to start this when the input changed
  // we just do the count over and over
  for (uint16_t i = 0; i < m_numPins; i++) {
    // if we read the same value as previous read, increment count
    if (((reading & m_curr_values) >> i) & 0x1) m_integratingDebounceCounts[i]++;
    // if not, decrement count
    else
        // avoid overflow
        if (m_integratingDebounceCounts[i] > 0)
      m_integratingDebounceCounts[i]--;

    // when there are m_integratingDebounceThres number of consecutive reads of the same
    // value, the value is valid, and we will use it
    if (m_integratingDebounceCounts[i] >= m_integratingDebounceThres) {
      m_values |= (reading & 1 << i);

      // clear the invalid bit
      m_invalidReads &= ~(1 << i);
    }
    // else if count of consecutive same reads has decremented to 0, the value is invalid
    else if (m_integratingDebounceCounts[i] == 0) {
      // set the invalid bit
      m_invalidReads |= (1 << i);
    }
  }

  m_curr_values = reading;
}

uint16_t DigitalInputGroup::getValuesAndInvalidReads(uint16_t& values) {
  values = m_values;

  // store a local copy of invalid reads, because reset() will clear m_invalidReads
  uint16_t invalidReads = m_invalidReads;
  reset();

  return invalidReads;
}

void DigitalInputGroup::reset() {
  m_values      = 0xFFFF;
  m_curr_values = 0xFFFF;

  m_delayStarted = 0;

  m_invalidReads = 0xFFFF;

  for (uint16_t i = 0; i < m_numPins; i++) {
    m_integratingDebounceCounts[i] = 0;
    m_startTimes[i]                = 0s;
  }
}
