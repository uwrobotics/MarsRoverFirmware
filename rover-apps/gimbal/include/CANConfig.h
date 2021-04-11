#pragma once

#include "CANInterface.h"
#include "CANMsg.h"
#include "hw_bridge.h"

static void sendACK(HWBRIDGE::GIMBAL_ACK_VALUES ackValue);
static mbed_error_status_t gimbalSetControlMode(CANMsg& msg);
static mbed_error_status_t gimbalSetJointPIDParams(CANMsg& msg);
static mbed_error_status_t commonSwitchCANBus(CANMsg& msg);

namespace CANConfig {

using namespace HWBRIDGE;

static CANMsgMap rxStreamedMsgMap = {
    {CANID::GIMBAL_SET_JOINT_POSITION,
     {
         {CANSIGNAL::GIMBAL_SET_PAN_POSITION, 0},
         {CANSIGNAL::GIMBAL_SET_PITCH_POSITION, 0},
         {CANSIGNAL::GIMBAL_SET_ROLL_POSITION, 0},
     }},
    {CANID::GIMBAL_SET_JOINT_ANGULAR_VELOCITY,
     {
         {CANSIGNAL::GIMBAL_SET_PAN_ANGULAR_VELOCITY, 0},
     }},
};

static CANMsgMap txStreamedMsgMap = {
    {CANID::GIMBAL_REPORT_JOINT_DATA,
     {
         {CANSIGNAL::GIMBAL_REPORT_PAN_POSITION, 0},
         {CANSIGNAL::GIMBAL_REPORT_PAN_ANGULAR_VELOCITY, 0},
     }},
    {CANID::GIMBAL_REPORT_FAULTS,
     {
         {CANSIGNAL::GIMBAL_PAN_ENCODER_STATE,
          (CANSignalValue_t)GIMBAL_PAN_ENCODER_STATE_VALUES::GIMBAL_PAN_ENCODER_STATE_SNA},
     }},
    {CANID::GIMBAL_REPORT_DIAGNOSTICS,
     {
         {CANSIGNAL::GIMBAL_REPORT_NUM_STREAMED_MSGS_RECEIVED, 0},
         {CANSIGNAL::GIMBAL_REPORT_NUM_ONE_SHOT_MSGS_RECEIVED, 0},
     }},
};

const static CANMsg::CANMsgHandlerMap rxOneShotMsgHandler = {
    {CANID::GIMBAL_SET_CONTROL_MODE, &gimbalSetControlMode},
    {CANID::GIMBAL_SET_JOINT_PID_PARAMS, &gimbalSetJointPIDParams},
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
