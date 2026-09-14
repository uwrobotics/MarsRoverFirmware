#pragma once

#include "CANInterface.h"
#include "CANMsg.h"
#include "hw_bridge.h"

namespace CANConfig {

using namespace HWBRIDGE;

static CANMsgMap rxMsgMap = {
    {CANID::COMMON_DEBUG_MESSAGE1,
     {
         {CANSIGNAL::COMMON_DEBUG_SIGNAL1, 0},
     }},
};

CANInterface::Config config = {
    // CAN bus pins
    .can1_RX = CAN1_RX,
    .can1_TX = CAN1_TX,
    .can2_RX = CAN2_RX,
    .can2_TX = CAN2_TX,

    // Message maps and handlers
    .rxMsgMap            = &rxMsgMap,
    .txMsgMap            = nullptr,
    .rxOneShotMsgHandler = nullptr,
};

}  // namespace CANConfig
