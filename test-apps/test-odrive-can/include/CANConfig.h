#pragma once

#include "CANInterface.h"
#include "CANMsg.h"
#include "hw_bridge.h"

static mbed_error_status_t handle_test_msg_one_shot(void);

namespace CANConfig {

using namespace HWBRIDGE;

static CANMsgMap rxMsgMap = {
    // Msg 1
    {CANID::ODRIVE_SET_INPUT_POS_AXIS0,
     {
         {CANSIGNAL::ODRIVE_INPUT_POS_AXIS0, 0},
         {CANSIGNAL::ODRIVE_VEL_FF_AXIS0, 0}, 
         {CANSIGNAL::ODRIVE_TORQUE_FF_POS_AXIS0, 0}
     }},

     {CANID::ODRIVE_SET_AXIS_REQUESTED_STATE_AXIS0,
     {
         {CANSIGNAL::ODRIVE_AXIS_REQUESTED_STATE_AXIS0,8}
     }},

    // Msg 2
    /*{CANID::COMMON_DEBUG_MESSAGE3,
     {
         {CANSIGNAL::COMMON_DEBUG_SIGNAL3, 0},
     }},*/
};

static CANMsgMap txMsgMap = {
    // Msg 1

    // Msg 2*/

    {CANID::ODRIVE_HEARTBEAT_MESSAGE_AXIS0,
     {
        //  {CANSIGNAL::ODRIVE_AXIS_ERROR_AXIS0, (CANSignalValue_t)ODRIVE_AXIS_ERROR_AXIS0_VALUES},
        //  {CANSIGNAL::ODRIVE_AXIS_CURRENT_STATE_AXIS0, (CANSignalValue_t)ODRIVE_AXIS_CURRENT_STATE_AXIS0_VALUES},
        //  {CANSIGNAL::ODRIVE_CONTROLLER_STATUS_AXIS0, (CANSignalValue_t)ODRIVE_CONTROLLER_STATUS_AXIS0_VALUES},
         {CANSIGNAL::ODRIVE_AXIS_ERROR_AXIS0, 0},
         {CANSIGNAL::ODRIVE_AXIS_CURRENT_STATE_AXIS0, 0},
         {CANSIGNAL::ODRIVE_CONTROLLER_STATUS_AXIS0, 0},
     }},
};

/*const static CANMsg::CANMsgHandlerMap rxOneShotMsgHandler = {
    {CANID::COMMON_DEBUG_MESSAGE3, &handle_test_msg_one_shot},
};*/

CANInterface::Config config = {
    // CAN bus pins
    .can1_RX = CAN1_RX,
    .can1_TX = CAN1_TX,
    .can2_RX = CAN2_RX,
    .can2_TX = CAN2_TX,

    // Message maps and handlers
    .rxMsgMap            = &rxMsgMap,
    .txMsgMap            = &txMsgMap,
    //.rxOneShotMsgHandler = &rxOneShotMsgHandler,
};

}  // namespace CANConfig
