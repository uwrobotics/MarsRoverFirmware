#pragma once

#include "CANInterface.h"
#include "CANMsg.h"
#include "hw_bridge.h"


namespace CANConfig {

using namespace HWBRIDGE;

static CANMsgMap rxMsgMap = {
    // Msg 1
    {CANID::COMMON_DEBUG_MESSAGE1,
     {
         {CANSIGNAL::COMMON_DEBUG_SIGNAL1, 0},
     }},
};



CANInterface::Config config = {
    // CAN bus pin
    .can1_RX = CAN1_RX,

    // Message maps and handlers
    .rxMsgMap            = &rxMsgMap,
};

}  // namespace CANConfig
