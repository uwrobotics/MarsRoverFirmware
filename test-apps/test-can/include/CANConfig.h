#pragma once

#include "CANInterface.h"
#include "CANMsg.h"
#include "hw_bridge.h"

static mbed_error_status_t handle_test_msg_one_shot(CANMsg& msg);

namespace CANConfig {

using namespace HWBRIDGE;

static CANMsgMap rxStreamedMsgMap = {
    // Msg 1
    {CANID::COMMON_DEBUG_MESSAGE1,
     {
         {CANSIGNAL::COMMON_DEBUG_SIGNAL1, 0},
     }},

    // Msg 2
};

static CANMsgMap txStreamedMsgMap = {
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
    .can1_RX = CAN2_RX,  // FOR TESTING, swap CAN1 and CAN2
    .can1_TX = CAN2_TX,
    .can2_RX = CAN1_RX,
    .can2_TX = CAN1_TX,

    // Message maps and handlers
    .rxStreamedMsgMap    = &rxStreamedMsgMap,
    .txStreamedMsgMap    = &txStreamedMsgMap,
    .rxOneShotMsgHandler = &rxOneShotMsgHandler,
};

}  // namespace CANConfig
