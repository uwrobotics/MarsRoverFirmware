#include "CANBus.h"

CANBus::CANBus(PinName rd, PinName td) : CAN(rd, td) {}

CANBus::CANBus(PinName rd, PinName td, uint32_t hz) : CAN(rd, td, hz) {}

int CANBus::setFilter(HWBRIDGE::CANFILTER canFilter, CANFormat format, uint16_t mask, int handle) {
  return CAN::filter(static_cast<uint16_t>(canFilter), mask, format, handle);
}

can_t *CANBus::getHandle() {
  return &_can;
}
