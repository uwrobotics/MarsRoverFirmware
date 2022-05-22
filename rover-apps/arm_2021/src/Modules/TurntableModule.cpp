#pragma once

#include "TurntableModule.h"

TurntableModule::TurntableModule()
    : turntableSetPoint(0),
      encoder(Encoder::AEAT6012({TRNTBL_ENC_SPI_CLK, TRNTBL_ENC_SPI_MISO, NC, 0})),
      motor(Actuator::DCMotor(MTR_PWM_TRNTBL, MTR_DIR_TRNTBL, false)),
      velPid(PID::PID({1, 0, 0, -1, 1, 0, false, false})),
      posPID(PID::PID({1, 0, 0, -1, 1, 0, false, false})),
      curPID(PID::PID({1, 0, 0, -1, 1, 0, false, false})),
      vel(Controller::Velocity(motor, encoder, std::nullopt, velPID, MAXDEGPERSEC, MAXCURRENT, LIM_TRNTBL_LHS,
                               LIM_TRNTBL_RHS)),
      pos(Controller::Position(motor, encoder, std::nullopt, posPID, MAXDEGPERSEC, MAXCURRENT, LIM_TRNTBL_LHS,
                               LIM_TRNTBL_RHS)),
      open(
          Controller::OpenLoop(motor, encoder, std::nullopt, MAXDEGPERSEC, MAXCURRENT, LIM_TRNTBL_LHS, LIM_TRNTBL_RHS)),
      manager(Controller::ActuatorControllerManager(Internal::lut, HWBRIDGE::CONTROL::Mode::OPEN_LOOP)) {}

void periodic_1ms(CANInterface can) override {
  switch (manager.getActiveControlMode()) {
    case HWBRIDGE::CONTROL::Mode::OPEN_LOOP:
    case HWBRIDGE::CONTROL::Mode::POSITION:
      can.getRXSignalValue(HWBRIDGE::CANID::ARM_SET_JOINT_POSITION, HWBRIDGE::CANSIGNAL::ARM_SET_TURNTABLE_POSITION,
                           turntableSetPoint);
      turntableSetPoint = RAD_TO_DEG(turntableSetPoint);
      break;

    case HWBRIDGE::CONTROL::Mode::VELOCITY:
      can.getRXSignalValue(HWBRIDGE::CANID::ARM_SET_JOINT_ANGULAR_VELOCITY,
                           HWBRIDGE::CANSIGNAL::ARM_SET_TURNTABLE_ANGULAR_VELOCITY, turntableSetPoint);
      turntableSetPoint = RAD_TO_DEG(turntableSetPoint);
      break;

    case HWBRIDGE::CONTROL::Mode::CURRENT:
      can.getRXSignalValue(HWBRIDGE::CANID::ARM_SET_JOINT_CURRENT, HWBRIDGE::CANSIGNAL::ARM_SET_TURNTABLE_CURRENT,
                           turntableSetPoint);
      break;

    default:
      break;
  }
}
