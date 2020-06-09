#pragma once

#include "mbed.h"

/*
        This class wraps a BusIn object. It is used for reading a group of digital inputs, and provides
   different types of debouncing to the read as specified
*/

enum class InputDebounceType { NONE, DELAY, INTERGRATING };

class DigitalInputGroup {
 public:
  /*
          Difference between delay debouncing and integrating debouncing:
          Delay: the value is valid, if it remains same after a delay interval;
          Integrating: the value is valid, if there are a number of consecutive reads of the same value.
          It increments a counter when the same value is read, and decrements it when the value is different.
          The benefits is this can efficiently "ignore" the initial oscillation in the signal when input changes.

  */

  DigitalInputGroup(BusIn& inputs, uint8_t numPins, InputDebounceType debounceType = InputDebounceType::NONE,
                    uint16_t integratingDebounceThres = DEFAULT_INTEGRATING_DEBOUNCE_THRES,
                    uint16_t delayDebouceTimeMS       = DEFAULT_DELAY_DEBOUNCE_MS);
  virtual ~DigitalInputGroup() = default;

  // some getters
  uint8_t getNumInputs() const;

  static constexpr uint8_t MAX_INPUTS_NUM = 16;

  /*
      Read the digital inputs, based on the debounce type selected. This function is intended to be
         called as often as possible, ideally in a tight loop
      */
  void read();

  /*
          Get the values of the digital inputs by reference, the returned value is a bit field of the
          invalid pin reads.
          As for what is considered as an invalid read, for delay debouncing, it is when delay time has
          not passed, or the read is different from the value before the delay (the read is a noise);
          for integrating debounce, it is when there is less than m_integratingDebounceThres consecutive\
          reads of the same value

          Note that the order of pins in the constructor of BusIn is the reverse order of the pins in the
          byte order. If you have BusIn(a,b,c,d,e,f,g,h) then the order of bits in the byte would be
          hgfedcba with a being bit 0, b being bit 1, c being bit 2 and so on.
  */
  uint16_t getValuesAndInvalidReads(uint16_t& values);

  /* reset the internal variables to start fresh, called by getValuesAndInvalidReads */
  void reset();

  /* static function that might be reused in other places */
  static inline bool interval_passed(uint32_t start_time, uint32_t current_time, uint16_t interval) {
    if (current_time > start_time) return (current_time - start_time) > interval;
    // else the timer has wrapped around
    else
      return (MAX_UINT32 - start_time + current_time) > interval;
  }

 protected:
  void readNoDebounce();
  void readDelayDebounce();
  void readIntegratingDebounce();
  static constexpr uint16_t DEFAULT_INTEGRATING_DEBOUNCE_THRES = 5;
  static constexpr uint16_t DEFAULT_DELAY_DEBOUNCE_MS          = 10;
  static constexpr uint32_t MAX_UINT32                         = 0xFFFFFFFF;

  BusIn& m_inputs;
  uint16_t m_numPins;

  // These two are bit fields, each bit corresponding to the value of one input
  // defaults to all 1s, assume that by default all digital inputs are active low
  uint16_t m_values      = 0xFFFF;
  uint16_t m_curr_values = 0xFFFF;

  // we make the array size big enough when declared. This might result in some waste, but this
  // allows us to not dynamically allocate memory
  uint16_t m_integratingDebounceCounts[MAX_INPUTS_NUM] = {0};
  uint16_t m_integratingDebounceThres                  = DEFAULT_INTEGRATING_DEBOUNCE_THRES;

  Timer m_timer;
  // This is a bit field, each bit corresponds to whether delay for one input has started or not
  uint16_t m_delayStarted               = 0;
  uint32_t m_startTimes[MAX_INPUTS_NUM] = {0};
  uint16_t m_delayDebounceTimeMS        = DEFAULT_DELAY_DEBOUNCE_MS;

  // Another bit field, each bit corresponds to whether the read from one input is valid or not
  // The default/initial value will be all 1s, because at the start, all inputs read should be invalid
  // as no readings has been done
  uint16_t m_invalidReads = 0xFFFF;

  InputDebounceType m_debounceType = InputDebounceType::NONE;
};