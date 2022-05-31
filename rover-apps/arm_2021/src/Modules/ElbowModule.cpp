#pragma once

#include "ElbowModule.h"

ElbowModule::ElbowModule()
    : elbowSetPoint(0),
      encoder(Encoder::AEAT6012({ELBW_ENC_SPI_CLK, ELBW_ENC_SPI_MISO, NC, 0})),
      motor(Actuator::DCMotor(MTR_PWM_ELBW, MTR_DIR_ELBW, false)),
      velPid(PID::PID({1, 0, 0, -1, 1, 0, false, false})),
      posPID(PID::PID({1, 0, 0, -1, 1, 0, false, false})),
      curPID(PID::PID({1, 0, 0, -1, 1, 0, false, false})),
      vel(Controller::Velocity(motor, encoder, std::nullopt, velPID, ASSUNMAXDEGPERSEC, ASSUNMAXCURRENT, LIM_ELBW_DN,
                                LIM_ELBW_UP)),
      pos(Controller::Position(motor, encoder, std::nullopt, posPID, ASSUNMAXDEGPERSEC, ASSUNMAXCURRENT, LIM_ELBW_DN,
                                LIM_ELBW_UP)),
      open(Controller::OpenLoop(motor, encoder, std::nullopt, ASSUNMAXDEGPERSEC, ASSUNMAXCURRENT, LIM_ELBW_DN,
                                 LIM_ELBW_UP)),
      manager(Controller::ActuatorControllerManager(lut, HWBRIDGE::CONTROL::Mode::OPEN_LOOP)) {}

void periodic_1ms(CANInterface& can) override {
  // Determine new elbow set point
    switch (Elbow::manager.getActiveControlMode()) {
      case HWBRIDGE::CONTROL::Mode::OPEN_LOOP:
      case HWBRIDGE::CONTROL::Mode::POSITION:
        can.getRXSignalValue(HWBRIDGE::CANID::ARM_SET_JOINT_POSITION, HWBRIDGE::CANSIGNAL::ARM_SET_ELBOW_POSITION,
                             elbowSetPoint);
        elbowSetPoint = RAD_TO_DEG(elbowSetPoint);
        break;

      case HWBRIDGE::CONTROL::Mode::VELOCITY:
        can.getRXSignalValue(HWBRIDGE::CANID::ARM_SET_JOINT_ANGULAR_VELOCITY,
                             HWBRIDGE::CANSIGNAL::ARM_SET_ELBOW_ANGULAR_VELOCITY, elbowSetPoint);
        elbowSetPoint = RAD_TO_DEG(elbowSetPoint);
        break;

      case HWBRIDGE::CONTROL::Mode::CURRENT:
        can.getRXSignalValue(HWBRIDGE::CANID::ARM_SET_JOINT_CURRENT, HWBRIDGE::CANSIGNAL::ARM_SET_ELBOW_CURRENT,
                             elbowSetPoint);
        break;

      default:
        break;
    }

Elbow::manager.getActiveController()->setSetPoint(static_cast<float>(elbowSetPoint));

    Elbow::manager.getActiveController()->update();
    can.setTXSignalValue(HWBRIDGE::CANID::ARM_REPORT_JOINT_POSITION, HWBRIDGE::CANSIGNAL::ARM_REPORT_ELBOW_POSITION,
                         DEG_TO_RAD(Elbow::manager.getActiveController()->reportAngleDeg()));

    can.setTXSignalValue(HWBRIDGE::CANID::ARM_REPORT_JOINT_ANGULAR_VELOCITY,
                         HWBRIDGE::CANSIGNAL::ARM_REPORT_ELBOW_ANGULAR_VELOCITY,
                         DEG_TO_RAD(Elbow::manager.getActiveController()->reportAngularVelocityDegPerSec()));


}
