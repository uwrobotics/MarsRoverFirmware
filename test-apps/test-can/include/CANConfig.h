#pragma once

#include "CANMsg.h"
#include "hw_bridge.h"

namespace TestCANConfig {

using namespace HWBRIDGE;

static mbed_error_status_t handle_switch_can_bus(CANMsg& msg);

static CANMsgMap rxStreamedMsgMap = {
    // Msg 1
    {COMMON_SWITCH_CAN_BUS,
     {
         {COMMON_CAN_BUS_ID, UWRT_MARS_ROVER_CAN_COMMON_SWITCH_CAN_BUS_COMMON_CAN_BUS_ID_SNA_CHOICE},
     }},

    // Msg 2
};

static CANMsgMap txStreamedMsgMap = {
    // Msg 1
    {COMMON_SWITCH_CAN_BUS,
     {
         {COMMON_CAN_BUS_ID, UWRT_MARS_ROVER_CAN_COMMON_SWITCH_CAN_BUS_COMMON_CAN_BUS_ID_SNA_CHOICE},
     }},

    // Msg 2
};

const static CANMsg::CANMsgHandlerMap rxOneShotMsgHandler = {
    {COMMON_SWITCH_CAN_BUS, &handle_switch_can_bus},
};

mbed_error_status_t handle_switch_can_bus(CANMsg& msg) {
  return MBED_ERROR_CODE_INVALID_ARGUMENT;
}

}  // namespace TestCANConfig
