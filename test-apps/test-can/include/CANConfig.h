#pragma once

#include "CANInterface.h"
#include "CANMsg.h"
#include "hw_bridge.h"

static mbed_error_status_t handle_test_msg_one_shot(CANMsg& msg);

namespace CANConfig {

using namespace HWBRIDGE;

static CANMsgMap rxStreamedMsgMap = {
    // Msg 1
    {CANID::COMMON_TEST_MESSAGE2,
     {
         {CANSIGNAL::COMMON_TEST_SIGNAL2, (CANSignalValue_t)COMMON_TEST_SIGNAL2_VALUES::COMMON_TEST_SIGNAL2_SNA},
         {CANSIGNAL::COMMON_TEST_SIGNAL3, (CANSignalValue_t)COMMON_TEST_SIGNAL3_VALUES::COMMON_TEST_SIGNAL3_SNA},
     }},

    // Msg 2
};

static CANMsgMap txStreamedMsgMap = {
    // Msg 1
    {CANID::COMMON_TEST_MESSAGE2,
     {
         {CANSIGNAL::COMMON_TEST_SIGNAL2, (CANSignalValue_t)COMMON_TEST_SIGNAL2_VALUES::COMMON_TEST_SIGNAL2_SNA},
         {CANSIGNAL::COMMON_TEST_SIGNAL3, (CANSignalValue_t)COMMON_TEST_SIGNAL3_VALUES::COMMON_TEST_SIGNAL3_SNA},
     }},

    // Msg 2
};

const static CANMsg::CANMsgHandlerMap rxOneShotMsgHandler = {
    {CANID::COMMON_TEST_MESSAGE1, &handle_test_msg_one_shot},
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
