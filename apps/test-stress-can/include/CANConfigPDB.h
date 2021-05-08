#pragma once

#include "CANInterface.h"
#include "CANMsg.h"
#include "hw_bridge.h"

const HWBRIDGE::CANFILTER targetCANIDFilter             = HWBRIDGE::CANFILTER::PDB_RX_FILTER;
const HWBRIDGE::CANID targetReportDiagnosticsCANID      = HWBRIDGE::CANID::PDB_REPORT_DIAGNOSTICS;
const HWBRIDGE::CANSIGNAL targetReportNumStreamedSignal = HWBRIDGE::CANSIGNAL::PDB_REPORT_NUM_STREAMED_MSGS_RECEIVED;
const HWBRIDGE::CANSIGNAL targetReportNumOneShotsSignal = HWBRIDGE::CANSIGNAL::PDB_REPORT_NUM_ONE_SHOT_MSGS_RECEIVED;
const HWBRIDGE::CANID targetReportFaultsCANID           = HWBRIDGE::CANID::PDB_REPORT_FAULTS;
const HWBRIDGE::CANSIGNAL targetNumCANRXFaultsSignal    = HWBRIDGE::CANSIGNAL::PDB_NUM_CANRX_FAULTS;
const HWBRIDGE::CANSIGNAL targetNumCANTXFaultsSignal    = HWBRIDGE::CANSIGNAL::PDB_NUM_CANTX_FAULTS;

static mbed_error_status_t oneShotHandler(void);
static mbed_error_status_t switchCANBus(void);

namespace CANConfig {

using namespace HWBRIDGE;

static CANMsgMap rxMsgMap = {
    // One-shot messages
    {CANID::PDB_SET_LED_MATRIX,
     {
         {CANSIGNAL::PDB_LED_MATRIX_STATE, (CANSignalValue_t)PDB_LED_MATRIX_STATE_VALUES::SNA},
     }},
    {CANID::COMMON_SWITCH_CAN_BUS,
     {
         {CANSIGNAL::COMMON_CAN_BUS_ID, (CANSignalValue_t)COMMON_CAN_BUS_ID_VALUES::SNA},
     }},
};

static CANMsgMap txMsgMap = {
    {CANID::PDB_REPORT_SENSOR_DATA,
     {
         {CANSIGNAL::PDB_ULTRASONIC_SENSOR1_DATA, 0},
         {CANSIGNAL::PDB_ULTRASONIC_SENSOR2_DATA, 0},
         {CANSIGNAL::PDB_ULTRASONIC_SENSOR3_DATA, 0},
         {CANSIGNAL::PDB_ULTRASONIC_SENSOR4_DATA, 0},
     }},
    {CANID::PDB_REPORT_FAULTS,
     {
         {CANSIGNAL::PDB_JETSON_STATUS, (CANSignalValue_t)PDB_JETSON_STATUS_VALUES::SNA},
         {CANSIGNAL::PDB_ARM_POWER_STATUS, (CANSignalValue_t)PDB_ARM_POWER_STATUS_VALUES::SNA},
         {CANSIGNAL::PDB_SCIENCE_POWER_STATUS, (CANSignalValue_t)PDB_SCIENCE_POWER_STATUS_VALUES::SNA},
         {CANSIGNAL::PDB_GIMBAL_POWER_STATUS, (CANSignalValue_t)PDB_GIMBAL_POWER_STATUS_VALUES::SNA},
         {CANSIGNAL::PDB_VBAT_RAIL_STATUS, (CANSignalValue_t)PDB_VBAT_RAIL_STATUS_VALUES::SNA},
         {CANSIGNAL::PDB_17_V_RAIL_STATUS, (CANSignalValue_t)PDB_17_V_RAIL_STATUS_VALUES::SNA},
         {CANSIGNAL::PDB_5_V_RAIL_STATUS, (CANSignalValue_t)PDB_5_V_RAIL_STATUS_VALUES::SNA},
         {CANSIGNAL::PDB_NUM_CANRX_FAULTS, 0},
         {CANSIGNAL::PDB_NUM_CANTX_FAULTS, 0},
     }},
    {CANID::PDB_REPORT_DIAGNOSTICS,
     {
         {CANSIGNAL::PDB_REPORT_NUM_STREAMED_MSGS_RECEIVED, 0},
         {CANSIGNAL::PDB_REPORT_NUM_ONE_SHOT_MSGS_RECEIVED, 0},
     }},
};

const static CANMsg::CANMsgHandlerMap rxOneShotMsgHandler = {
    {CANID::PDB_SET_LED_MATRIX, &oneShotHandler},
    {CANID::COMMON_SWITCH_CAN_BUS, &switchCANBus},
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
