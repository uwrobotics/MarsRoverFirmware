#pragma once

#include "CANInterface.h"
#include "CANMsg.h"
#include "hw_bridge.h"

static mbed_error_status_t handle_test_msg_one_shot(void);

namespace CANConfig {

using namespace HWBRIDGE;

static CANMsgMap rxMsgMap = {
    // Msg 1
    {CANID::COMMON_DEBUG_MESSAGE1,
     {
         {CANSIGNAL::COMMON_DEBUG_SIGNAL1, 0},
     }},

    // Msg 2
    {CANID::COMMON_DEBUG_MESSAGE3,
     {
         {CANSIGNAL::COMMON_DEBUG_SIGNAL3, 0},
     }},
};

static CANMsgMap txMsgMap = {
    // Msg 1
    {CANID::COMMON_DEBUG_MESSAGE2,
     {
         {CANSIGNAL::COMMON_DEBUG_SIGNAL2, 0},
     }},

    // Msg 2
};

const static CANMsg::CANMsgHandlerMap rxOneShotMsgHandler = {
    {CANID::COMMON_DEBUG_MESSAGE3, &handle_test_msg_one_shot},
};

CANInterface::Config config = {
    // CAN bus pins
    .can1_RX = CAN1_RX,
    .can1_TX = CAN1_TX,
    .can2_RX = CAN2_RX,
    .can2_TX = CAN2_TX,

    // Message maps and handlers
    .rxMsgMap            = &rxMsgMap,
    .txMsgMap            = &txMsgMap,
    .rxOneShotMsgHandler = &rxOneShotMsgHandler,
};

}  // namespace CANConfig
