#pragma once

#include "CANInterface.h"
#include "CANMsg.h"
#include "hw_bridge.h"

static void sendACK(HWBRIDGE::GIMBAL_ACK_VALUES ackValue);
static mbed_error_status_t gimbalSetControlMode(CANMsg& msg);

namespace CANConfig {

using namespace HWBRIDGE;

static CANMsgMap rxStreamedMsgMap = {
    {CANID::GIMBAL_SET_JOINT_POSITION,
     {
         {CANSIGNAL::GIMBAL_SET_PAN_POSITION,
          (CANSignalValue_t)GIMBAL_SET_PAN_POSITION_VALUES::GIMBAL_SET_PAN_POSITION_SNA},
         {CANSIGNAL::GIMBAL_SET_PITCH_POSITION,
          (CANSignalValue_t)GIMBAL_SET_PITCH_POSITION_VALUES::GIMBAL_SET_PITCH_POSITION_SNA},
         {CANSIGNAL::GIMBAL_SET_ROLL_POSITION,
          (CANSignalValue_t)GIMBAL_SET_ROLL_POSITION_VALUES::GIMBAL_SET_ROLL_POSITION_SNA},
     }},
    {CANID::GIMBAL_SET_JOINT_ANGULAR_VELOCITY,
     {
         {CANSIGNAL::GIMBAL_SET_PAN_ANGULAR_VELOCITY,
          (CANSignalValue_t)GIMBAL_SET_PAN_ANGULAR_VELOCITY_VALUES::GIMBAL_SET_PAN_ANGULAR_VELOCITY_SNA},
     }},
};

static CANMsgMap txStreamedMsgMap = {
    {CANID::GIMBAL_REPORT_JOINT_DATA,
     {
         {CANSIGNAL::GIMBAL_REPORT_PAN_POSITION,
          (CANSignalValue_t)GIMBAL_REPORT_PAN_POSITION_VALUES::GIMBAL_REPORT_PAN_POSITION_SNA},
         {CANSIGNAL::GIMBAL_REPORT_PAN_ANGULAR_VELOCITY,
          (CANSignalValue_t)GIMBAL_REPORT_PAN_ANGULAR_VELOCITY_VALUES::GIMBAL_REPORT_PAN_ANGULAR_VELOCITY_SNA},
     }},
    {CANID::GIMBAL_REPORT_FAULTS,
     {
         {CANSIGNAL::GIMBAL_PAN_ENCODER_STATE,
          (CANSignalValue_t)GIMBAL_PAN_ENCODER_STATE_VALUES::GIMBAL_PAN_ENCODER_STATE_SNA},
     }},
};

const static CANMsg::CANMsgHandlerMap rxOneShotMsgHandler = {
    {CANID::GIMBAL_SET_CONTROL_MODE, &gimbalSetControlMode},
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
