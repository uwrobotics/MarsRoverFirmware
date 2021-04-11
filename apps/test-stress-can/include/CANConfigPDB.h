#pragma once

#include "CANInterface.h"
#include "CANMsg.h"
#include "hw_bridge.h"

const HWBRIDGE::CANFILTER targetCANIDFilter             = HWBRIDGE::CANFILTER::NO_MESSAGES;
const HWBRIDGE::CANID targetReportDiagnosticsCANID      = HWBRIDGE::CANID::PDB_REPORT_DIAGNOSTICS;
const HWBRIDGE::CANSIGNAL targetReportNumStreamedSignal = HWBRIDGE::CANSIGNAL::PDB_REPORT_NUM_STREAMED_MSGS_RECEIVED;
const HWBRIDGE::CANSIGNAL targetReportNumOneShotsSignal = HWBRIDGE::CANSIGNAL::PDB_REPORT_NUM_ONE_SHOT_MSGS_RECEIVED;

static mbed_error_status_t oneShotHandler(CANMsg& msg);

namespace CANConfig {

using namespace HWBRIDGE;

static CANMsgMap txStreamedMsgMap = {
    {CANID::PDB_REPORT_SENSOR_DATA,
     {
         {CANSIGNAL::PDB_ULTRASONIC_SENSOR1_DATA, 0},
         {CANSIGNAL::PDB_ULTRASONIC_SENSOR2_DATA, 0},
         {CANSIGNAL::PDB_ULTRASONIC_SENSOR3_DATA, 0},
         {CANSIGNAL::PDB_ULTRASONIC_SENSOR4_DATA, 0},
     }},
    {CANID::PDB_REPORT_FAULTS,
     {
         {CANSIGNAL::PDB_JETSON_STATUS, (CANSignalValue_t)PDB_JETSON_STATUS_VALUES::PDB_JETSON_STATUS_SNA},
         {CANSIGNAL::PDB_ARM_POWER_STATUS, (CANSignalValue_t)PDB_ARM_POWER_STATUS_VALUES::PDB_ARM_POWER_STATUS_SNA},
         {CANSIGNAL::PDB_SCIENCE_POWER_STATUS,
          (CANSignalValue_t)PDB_SCIENCE_POWER_STATUS_VALUES::PDB_SCIENCE_POWER_STATUS_SNA},
         {CANSIGNAL::PDB_GIMBAL_POWER_STATUS,
          (CANSignalValue_t)PDB_GIMBAL_POWER_STATUS_VALUES::PDB_GIMBAL_POWER_STATUS_SNA},
         {CANSIGNAL::PDB_VBAT_RAIL_STATUS, (CANSignalValue_t)PDB_VBAT_RAIL_STATUS_VALUES::PDB_VBAT_RAIL_STATUS_SNA},
         {CANSIGNAL::PDB_17_V_RAIL_STATUS, (CANSignalValue_t)PDB_17_V_RAIL_STATUS_VALUES::PDB_17_V_RAIL_STATUS_SNA},
         {CANSIGNAL::PDB_5_V_RAIL_STATUS, (CANSignalValue_t)PDB_5_V_RAIL_STATUS_VALUES::PDB_5_V_RAIL_STATUS_SNA},
     }},
    {CANID::PDB_REPORT_DIAGNOSTICS,
     {
         {CANSIGNAL::PDB_REPORT_NUM_STREAMED_MSGS_RECEIVED, 0},
         {CANSIGNAL::PDB_REPORT_NUM_ONE_SHOT_MSGS_RECEIVED, 0},
     }},
};

const static CANMsg::CANMsgHandlerMap rxOneShotMsgHandler = {
    {CANID::PDB_SET_LED_MATRIX, &oneShotHandler},
    {CANID::COMMON_SWITCH_CAN_BUS, &oneShotHandler},
};

CANInterface::Config config = {
    // CAN bus pins
    .can1_RX = CAN2_RX,  // TODO: revert this
    .can1_TX = CAN2_TX,
    .can2_RX = CAN1_RX,
    .can2_TX = CAN1_TX,

    // Message maps and handlers
    .rxStreamedMsgMap    = nullptr,
    .txStreamedMsgMap    = &txStreamedMsgMap,
    .rxOneShotMsgHandler = &rxOneShotMsgHandler,
};

}  // namespace CANConfig
