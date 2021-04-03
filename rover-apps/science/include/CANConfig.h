#pragma once

#include "CANInterface.h"
#include "CANMsg.h"
#include "hw_bridge.h"

static void sendACK(HWBRIDGE::SCIENCE_ACK_VALUES ackValue);
static mbed_error_status_t scienceSetControlMode(CANMsg& msg);
static mbed_error_status_t scienceSetJointPIDParams(CANMsg& msg);
static mbed_error_status_t commonSwitchCANBus(CANMsg& msg);

namespace CANConfig {

using namespace HWBRIDGE;

static CANMsgMap rxStreamedMsgMap = {
    {CANID::SCIENCE_SET_JOINT_POSITION,
     {
         {CANSIGNAL::SCIENCE_SET_GENEVA_POSITION, 0},
         {CANSIGNAL::SCIENCE_SET_ELEVATOR_POSITION, 0},
         {CANSIGNAL::SCIENCE_SET_COVER_POSITION, 0},
         {CANSIGNAL::SCIENCE_SET_SHOVEL_POSITION, 0},
     }},
    {CANID::SCIENCE_SET_JOINT_ANGULAR_VELOCITY,
     {
         {CANSIGNAL::SCIENCE_SET_GENEVA_ANGULAR_VELOCITY, 0},
     }},
};

static CANMsgMap txStreamedMsgMap = {
    {CANID::SCIENCE_REPORT_JOINT_DATA,
     {
         {CANSIGNAL::SCIENCE_REPORT_GENEVA_POSITION, 0},
         {CANSIGNAL::SCIENCE_REPORT_ELEVATOR_POSITION, 0},
         {CANSIGNAL::SCIENCE_REPORT_GENEVA_ANGULAR_VELOCITY, 0},
         {CANSIGNAL::SCIENCE_REPORT_ELEVATOR_ANGULAR_VELOCITY, 0},
     }},
    {CANID::SCIENCE_REPORT_SENSOR_DATA,
     {
         {CANSIGNAL::SCIENCE_MOISTURE_DATA, 0},
         {CANSIGNAL::SCIENCE_TEMPERATURE_DATA, 0},
     }},
    {CANID::SCIENCE_REPORT_FAULTS,
     {
         {CANSIGNAL::SCIENCE_GENEVA_ENCODER_STATE,
          (CANSignalValue_t)SCIENCE_GENEVA_ENCODER_STATE_VALUES::SCIENCE_GENEVA_ENCODER_STATE_SNA},
         {CANSIGNAL::SCIENCE_ELEVATOR_ENCODER_STATE,
          (CANSignalValue_t)SCIENCE_ELEVATOR_ENCODER_STATE_VALUES::SCIENCE_ELEVATOR_ENCODER_STATE_SNA},
         {CANSIGNAL::SCIENCE_MOISTURE_SENSOR_STATE,
          (CANSignalValue_t)SCIENCE_MOISTURE_SENSOR_STATE_VALUES::SCIENCE_MOISTURE_SENSOR_STATE_SNA},
     }},
};

const static CANMsg::CANMsgHandlerMap rxOneShotMsgHandler = {
    {CANID::SCIENCE_SET_CONTROL_MODE, &scienceSetControlMode},
    {CANID::SCIENCE_SET_JOINT_PID_PARAMS, &scienceSetJointPIDParams},
    {CANID::COMMON_SWITCH_CAN_BUS, &commonSwitchCANBus},
};

CANInterface::Config config = {
    // CAN bus pins
    .can1_RX = CAN1_RX,
    .can1_TX = CAN1_TX,
    .can2_RX = CAN2_RX,
    .can2_TX = CAN2_TX,

    // Message maps and handlers
    .rxStreamedMsgMap    = &rxStreamedMsgMap,
    .txStreamedMsgMap    = &txStreamedMsgMap,
    .rxOneShotMsgHandler = &rxOneShotMsgHandler,
};

}  // namespace CANConfig
