#pragma once

#include "ClawModule.h"

ClawModule::ClawModule()
    : clawSetPoint(0),
      encoder(Encoder::Quadrature64CPR({ENC_QUAD_CLAW_A, ENC_QUAD_CLAW_B, NC, 0})),
      motor(Actuator::DCMotor(MTR_PWM_TRNTBL, MTR_DIR_TRNTBL, false)),
      velPid(PID::PID({1, 0, 0, -1, 1, 0, false, false})),
      posPID(PID::PID({1, 0, 0, -1, 1, 0, false, false})),
      curPID(PID::PID({1, 0, 0, -1, 1, 0, false, false})),
      vel(Controller::Velocity(motor, encoder, std::nullopt, velPID, POLOLUMADEGPERSEC, POLOLUMAXCURRENT, LIM_CLAW_OPEN,
                               NC)),
      pos(Controller::Position(motor, encoder, std::nullopt, posPID, POLOLUMADEGPERSEC, POLOLUMAXCURRENT, LIM_CLAW_OPEN,
                               NC)),
      open(Controller::OpenLoop(motor, encoder, std::nullopt, POLOLUMADEGPERSEC, POLOLUMAXCURRENT, LIM_CLAW_OPEN, NC)),
      manager(Controller::ActuatorControllerManager(lut, HWBRIDGE::CONTROL::Mode::OPEN_LOOP)) {}

void periodic_1ms(CANInterface& can) override {
  // Determine new claw set point
  switch (Claw::manager.getActiveControlMode()) {
    case HWBRIDGE::CONTROL::Mode::OPEN_LOOP:
    case HWBRIDGE::CONTROL::Mode::POSITION:
      can.getRXSignalValue(HWBRIDGE::CANID::ARM_SET_JOINT_POSITION, HWBRIDGE::CANSIGNAL::ARM_SET_CLAW_POSITION,
                           clawSetPoint);
      clawSetPoint = RAD_TO_DEG(clawSetPoint);
      break;

    case HWBRIDGE::CONTROL::Mode::VELOCITY:
      can.getRXSignalValue(HWBRIDGE::CANID::ARM_SET_JOINT_ANGULAR_VELOCITY,
                           HWBRIDGE::CANSIGNAL::ARM_SET_CLAW_ANGULAR_VELOCITY, clawSetPoint);
      clawSetPoint = RAD_TO_DEG(clawSetPoint);
      break;

    case HWBRIDGE::CONTROL::Mode::CURRENT:
      can.getRXSignalValue(HWBRIDGE::CANID::ARM_SET_JOINT_CURRENT, HWBRIDGE::CANSIGNAL::ARM_SET_CLAW_CURRENT,
                           clawSetPoint);
      break;

    default:
      break;
  }

  Claw::manager.getActiveController()->setSetPoint(static_cast<float>(clawSetPoint));
  Claw::manager.getActiveController()->update();

  can.setTXSignalValue(HWBRIDGE::CANID::ARM_REPORT_JOINT_POSITION, HWBRIDGE::CANSIGNAL::ARM_REPORT_CLAW_POSITION,
                       DEG_TO_RAD(Claw::manager.getActiveController()->reportAngleDeg()));

  can.setTXSignalValue(HWBRIDGE::CANID::ARM_REPORT_JOINT_ANGULAR_VELOCITY,
                       HWBRIDGE::CANSIGNAL::ARM_REPORT_CLAW_ANGULAR_VELOCITY,
                       DEG_TO_RAD(Claw::manager.getActiveController()->reportAngularVelocityDegPerSec()));
}
