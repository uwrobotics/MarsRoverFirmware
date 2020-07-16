#pragma once

#include <memory>

#include "mbed.h"

/* This class is analogous to the BusIn class (which can be found at mbed-os/drivers/BusIn)
   that mbed provided, except that we changed maximum number of inputs from 16 to 8

   A analog input bus, used for reading the state of a collection of pins

   @note Synchronization level: Thread safe
*/
class AnalogBusIn {
  // have to declare it at front, else won't be able to use it in function parameter
 private:
  static constexpr uint8_t MAX_INPUTS_NUM = 8;

 public:
  /** Create an AnalogBusIn, connected to the specified pins
   * @note
   *  It is only required to specify as many pin variables as is required
   *  for the bus; the rest will default to NC (not connected)
   */
  AnalogBusIn(PinName p0, PinName p1 = NC, PinName p2 = NC, PinName p3 = NC, PinName p4 = NC, PinName p5 = NC,
              PinName p6 = NC, PinName p7 = NC);

  /** Create an AnalogBusIn, connected to the specified pins
   *
   * @param pins An array of pins to connect to bus bit
   */
  AnalogBusIn(PinName pins[MAX_INPUTS_NUM]);

  virtual ~AnalogBusIn();

  /** Read the value of the input bus, represented as an array of floats in the range [0, 1]
   *	@param values, an array of floats of length 16 that will be filled with the values read
   */
  void read(float values[MAX_INPUTS_NUM]);

  /** Read the value of the input bus, represented as an array of unsigned short in the range [0x0, 0xFFFF]
   *	@param values, an array of unsigned short of length 16 that will be filled with the values read
   */
  void read_u16(uint16_t values[MAX_INPUTS_NUM]);

  /** Binary mask of bus pins connected to actual pins (not NC pins)
   *  If bus pin is in NC state make corresponding bit will be cleared (set to 0), else bit will be set to 1
   *
   *  @returns
   *    Binary mask of connected pins
   */
  int mask() {
    // No lock needed since _nc_mask is not modified outside the constructor
    return _nc_mask;
  }

  /** Access to particular input in random-iterator fashion
   * @param index  Position of input
   */
  AnalogIn &operator[](int index);

 protected:
  std::unique_ptr<AnalogIn> _pin[MAX_INPUTS_NUM];

  /* Mask of bus's NC pins
   * If bit[n] is set to 1 - pin is connected
   * if bit[n] is cleared - pin is not connected (NC)
   */
  int _nc_mask;

  PlatformMutex _mutex;

 private:
  virtual void lock();
  virtual void unlock();
};
