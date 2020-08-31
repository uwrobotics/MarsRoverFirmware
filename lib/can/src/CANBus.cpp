#include "CANBus.h"

CANBus::CANBus(PinName rd, PinName td) : CAN(rd, td) {}
CANBus::CANBus(PinName rd, PinName td, int hz) : CAN(rd, td, hz) {}
int CANBus::setFilter(HWBRIDGE::CANFILTER canFilter, uint16_t mask, CANFormat format, int handle) {
  return CAN::filter((uint16_t)canFilter, mask, format, handle);
}
