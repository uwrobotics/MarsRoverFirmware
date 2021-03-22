#pragma once

#include <vector>

#include "LookupTable.h"
#include "hw_bridge.h"

enum class CAN_SIGNAL_NAME { COMMON_CAN_BUS_ID };

class CANSignal {
 public:
  using CANSignalMap = Utility::LookupTable<uint16_t, std::vector<CANSignal>>;

  CAN_SIGNAL_NAME getName() {
    return name;
  }

  void setValue(double value) {
    this->value = value;
  }

  double getValue(void) {
    return this->value;
  }

 private:
  CAN_SIGNAL_NAME name;
  double value;
};
