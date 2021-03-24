#pragma once

#include "CANMsg.h"
#include "hw_bridge.h"

namespace TestCAN {

using namespace HWBRIDGE;

static mbed_error_status_t handle_switch_can_bus(CANMsg& msg);

static CANMsgMap rxStreamedMsgMap = {
    // Msg 1
    {UWRT_MARS_ROVER_CAN_COMMON_SWITCH_CAN_BUS_FRAME_ID,
     {
         {COMMON_CAN_BUS_ID, UWRT_MARS_ROVER_CAN_COMMON_SWITCH_CAN_BUS_COMMON_CAN_BUS_ID_SNA_CHOICE},
     }},

    // Msg 2
};

static CANMsgMap txStreamedMsgMap = {
    // Msg 1
    {UWRT_MARS_ROVER_CAN_COMMON_SWITCH_CAN_BUS_FRAME_ID,
     {
         {COMMON_CAN_BUS_ID, UWRT_MARS_ROVER_CAN_COMMON_SWITCH_CAN_BUS_COMMON_CAN_BUS_ID_SNA_CHOICE},
     }},

    // Msg 2
};

const static CANMsg::CANMsgHandlerMap rxOneShotMsgHandler = {
    {(HWBRIDGE::CANID)UWRT_MARS_ROVER_CAN_COMMON_SWITCH_CAN_BUS_FRAME_ID, &handle_switch_can_bus},
};

mbed_error_status_t handle_switch_can_bus(CANMsg& msg) {
  return MBED_ERROR_CODE_INVALID_ARGUMENT;
}

}  // namespace TestCAN
