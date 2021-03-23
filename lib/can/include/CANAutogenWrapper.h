#pragma once

#include <unordered_map>

#include "LookupTable.h"
#include "hw_bridge.h"

using CANSignalLUT = std::unordered_map<HWBRIDGE::CANSIGNALNAME, HWBRIDGE::CANSignalValue_t>;
using CANSignalMap = std::unordered_map<HWBRIDGE::CANMsgID_t, CANSignalLUT>;

bool packCANMsg(uint8_t* raw, HWBRIDGE::CANMsgID_t msgID, const CANSignalMap* signalMap);
bool unpackCANMsg(uint8_t* raw, HWBRIDGE::CANMsgID_t msgID, CANSignalMap* signalMap);
