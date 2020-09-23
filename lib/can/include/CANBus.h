#pragma once

#include "hw_bridge.h"

class CANBus : public CAN {
 public:
  /* Initialize CAN interface pins
   */
  CANBus(PinName rd, PinName td);
  /* Initialize CAN interface pins and frequency
   */
  CANBus(PinName rd, PinName td, int hz);
  /* Filter out incoming CAN msgs
   */
  int setFilter(HWBRIDGE::CANFILTER filter, CANFormat format = CANAny,
                uint16_t mask = HWBRIDGE::ROVERCONFIG::ROVER_CANID_FILTER_MASK, int handle = 0);

 private:
  // remove public access to CAN::filter
  using CAN::filter;
};
