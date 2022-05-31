#pragma once

#include "ShoulderModule.h"

ShoulderModule::ShoulderModule()
    : turntableSetPoint(0),
      encoder(Encoder::AEAT6012({SHLDR_ENC_SPI_CLK, SHLDR_ENC_SPI_MISO, SHLDR_ENC_SPI_CS, 0})),
      motor(Actuator::DCMotor(MTR_PWM_SHLDR, MTR_DIR_SHLDR, false)),
      velPid(PID::PID({1, 0, 0, -1, 1, 0, false, false})),
      posPID(PID::PID({1, 0, 0, -1, 1, 0, false, false})),
      curPID(PID::PID({1, 0, 0, -1, 1, 0, false, false})),
      vel(Controller::Velocity(motor, encoder, std::nullopt, velPID, PA04MAXDEGPERSEC, PA04MAXCURRENT, LIM_SHLDR_DN,
                               LIM_SHLDR_UP)),
      pos(Controller::Position(motor, encoder, std::nullopt, posPID, PA04MAXDEGPERSEC, PA04MAXCURRENT, LIM_SHLDR_DN,
                               LIM_SHLDR_UP)),
      open(Controller::OpenLoop(motor, encoder, std::nullopt, PA04MAXDEGPERSEC, PA04MAXCURRENT, LIM_SHLDR_DN,
                                LIM_SHLDR_UP)),
      manager(Controller::ActuatorControllerManager(lut, HWBRIDGE::CONTROL::Mode::OPEN_LOOP)) {}

void periodic_1ms(CANInterface& can) override {
  // Determine new shoulder set point
  switch (Shoulder::manager.getActiveControlMode()) {
    case HWBRIDGE::CONTROL::Mode::OPEN_LOOP:
    case HWBRIDGE::CONTROL::Mode::POSITION:
      can.getRXSignalValue(HWBRIDGE::CANID::ARM_SET_JOINT_POSITION, HWBRIDGE::CANSIGNAL::ARM_SET_SHOULDER_POSITION,
                           shoulderSetPoint);
      shoulderSetPoint = RAD_TO_DEG(shoulderSetPoint);
      break;

    case HWBRIDGE::CONTROL::Mode::VELOCITY:
      can.getRXSignalValue(HWBRIDGE::CANID::ARM_SET_JOINT_ANGULAR_VELOCITY,
                           HWBRIDGE::CANSIGNAL::ARM_SET_SHOULDER_ANGULAR_VELOCITY, shoulderSetPoint);
      shoulderSetPoint = RAD_TO_DEG(shoulderSetPoint);
      break;

    case HWBRIDGE::CONTROL::Mode::CURRENT:
      can.getRXSignalValue(HWBRIDGE::CANID::ARM_SET_JOINT_CURRENT, HWBRIDGE::CANSIGNAL::ARM_SET_SHOULDER_CURRENT,
                           shoulderSetPoint);
      break;

    default:
      break;
  }

  manager.getActiveController()->setSetPoint(static_cast<float>(shoulderSetPoint));
  manager.getActiveController()->update();
  can.setTXSignalValue(HWBRIDGE::CANID::ARM_REPORT_JOINT_POSITION, HWBRIDGE::CANSIGNAL::ARM_REPORT_SHOULDER_POSITION,
                       DEG_TO_RAD(manager.getActiveController()->reportAngleDeg()));
  can.setTXSignalValue(HWBRIDGE::CANID::ARM_REPORT_JOINT_ANGULAR_VELOCITY,
                       HWBRIDGE::CANSIGNAL::ARM_REPORT_SHOULDER_ANGULAR_VELOCITY,
                       DEG_TO_RAD(manager.getActiveController()->reportAngularVelocityDegPerSec()));
}
