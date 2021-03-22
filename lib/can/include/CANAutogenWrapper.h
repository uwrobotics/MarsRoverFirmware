#pragma once

#include "CANSignal.h"
#include "hw_bridge.h"

void packCANMsg(uint8_t* raw, uint16_t msgID, const std::vector<CANSignal>* signals);
