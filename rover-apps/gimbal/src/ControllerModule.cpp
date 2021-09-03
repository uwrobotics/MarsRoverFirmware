#pragma once

#include "Module.h"
#include "mbed.h"
#include "CANConfig.h"
#include "CANMsg.h"
#include "Logger.h"
#include "PanConfig.h"
#include "PinNames.h"
#include "PitchConfig.h"
#include "hw_bridge.h"
#include "ControllerModule.h"

CANInterface can(CANConfig::config);

// Convert degrees to radians
static inline float DEG_TO_RAD(float deg) {
  return deg * M_PI / 180.0f;
}

// Convert radians to degrees
static inline float RAD_TO_DEG(float rad) {
  return rad * 180.0f / M_PI;
}

ControllerModule::ControllerModule(CANInterface &can): m_can(&can){
    // Set CAN filters
    Utility::logger << "";  // Band-aid fix for logger bug (issue #328)
    m_can->setFilter(HWBRIDGE::CANFILTER::GIMBAL_RX_FILTER, CANStandard, HWBRIDGE::ROVER_CANID_FILTER_MASK, 0);
    m_can->setFilter(HWBRIDGE::CANFILTER::COMMON_FILTER, CANStandard, HWBRIDGE::ROVER_CANID_FILTER_MASK, 1);
}

void ControllerModule::periodic_1ms(void) {
    switch (Pan::manager.getActiveControlMode()) {
        case HWBRIDGE::CONTROL::Mode::OPEN_LOOP:
        case HWBRIDGE::CONTROL::Mode::POSITION:
            m_can->getRXSignalValue(HWBRIDGE::CANID::GIMBAL_SET_JOINT_POSITION, HWBRIDGE::CANSIGNAL::GIMBAL_SET_PAN_POSITION,
                                panSetPoint);
            panSetPoint = RAD_TO_DEG(panSetPoint);
            break;
        case HWBRIDGE::CONTROL::Mode::VELOCITY:
            m_can->getRXSignalValue(HWBRIDGE::CANID::GIMBAL_SET_JOINT_ANGULAR_VELOCITY,
                                HWBRIDGE::CANSIGNAL::GIMBAL_SET_PAN_ANGULAR_VELOCITY, panSetPoint);
            panSetPoint = RAD_TO_DEG(panSetPoint);
            break;
        default:
            break;
    }

    // Determine new pitch position
    m_can->getRXSignalValue(HWBRIDGE::CANID::GIMBAL_SET_JOINT_POSITION, HWBRIDGE::CANSIGNAL::GIMBAL_SET_PITCH_POSITION,
                        pitchPosition);
    pitchPosition = RAD_TO_DEG(pitchPosition);

    // *** UPDATE JOINT SET POINTS ***
    Pan::manager.getActiveController()->setSetPoint((float)panSetPoint);
    Pitch::pitchServo.setValue((float)pitchPosition);

    // *** COMPUTE ACTUATOR CONTROLS ***
    Pan::manager.getActiveController()->update();

    // *** UPDATE TX SIGNALS ***
    m_can->setTXSignalValue(HWBRIDGE::CANID::GIMBAL_REPORT_JOINT_DATA, HWBRIDGE::CANSIGNAL::GIMBAL_REPORT_PAN_POSITION,
                        DEG_TO_RAD(Pan::manager.getActiveController()->reportAngleDeg()));

    m_can->setTXSignalValue(HWBRIDGE::CANID::GIMBAL_REPORT_JOINT_DATA,
                        HWBRIDGE::CANSIGNAL::GIMBAL_REPORT_PAN_ANGULAR_VELOCITY,
                        DEG_TO_RAD(Pan::manager.getActiveController()->reportAngularVelocityDegPerSec()));

    // TODO: REPORT FAULTS
    m_can->setTXSignalValue(HWBRIDGE::CANID::GIMBAL_REPORT_FAULTS, HWBRIDGE::CANSIGNAL::GIMBAL_NUM_CANRX_FAULTS,
                        m_can->getNumCANRXFaults());
    m_can->setTXSignalValue(HWBRIDGE::CANID::GIMBAL_REPORT_FAULTS, HWBRIDGE::CANSIGNAL::GIMBAL_NUM_CANTX_FAULTS,
                        m_can->getNumCANTXFaults());

    // Report diagnostics
    m_can->setTXSignalValue(HWBRIDGE::CANID::GIMBAL_REPORT_DIAGNOSTICS,
                        HWBRIDGE::CANSIGNAL::GIMBAL_REPORT_NUM_STREAMED_MSGS_RECEIVED,
                        m_can->getNumStreamedMsgsReceived());
    m_can->setTXSignalValue(HWBRIDGE::CANID::GIMBAL_REPORT_DIAGNOSTICS,
                        HWBRIDGE::CANSIGNAL::GIMBAL_REPORT_NUM_ONE_SHOT_MSGS_RECEIVED,
                        m_can->getNumOneShotMsgsReceived());
}
