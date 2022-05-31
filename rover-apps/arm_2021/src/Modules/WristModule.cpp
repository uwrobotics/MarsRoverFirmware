#pragma once

#include "ElbowModule.h"

ElbowModule::ElbowModule()
    : elbowSetPoint(0),
      leftEncoder(Encoder::Quadrature64CPR({ENC_QUAD_WRST_LHS_A, ENC_QUAD_WRST_LHS_B, NC, 0})),
      leftEncoder(Encoder::Quadrature64CPR({ENC_QUAD_WRST_RHS_A, ENC_QUAD_WRST_RHS_B, NC, 0})),
      leftMotor(Actuator::DCMotor(MTR_PWM_WRST_LHS, MTR_DIR_WRST_LHS, false)),
      rightMotor(Actuator::DCMotor(MTR_PWM_WRST_RHS, MTR_DIR_WRST_RHS, false)),
/* TODO: Can we use wrist limit switches if motors are individually addressable */

// static Sensor::CurrentSensor leftCurrentSensor(WRSTL_CRNT_SNS_SPI_CLK, WRSTL_CRNT_SNS_SPI_MISO,
// WRSTL_CRNT_SNS_SPI_CS),
//     rightCurrentSensor(WRSTR_CRNT_SNS_SPI_CLK, WRSTR_CRNT_SNS_SPI_MISO, WRSTR_CRNT_SNS_SPI_CS);
// TODO: Add once current sensor driver works

      leftVelPID(PID::PID({1, 0, 0, -1, 1, 0, false, false})),
      leftPosPID(PID::PID({1, 0, 0, -1, 1, 0, false, false})),
      leftCurPID(PID::PID({1, 0, 0, -1, 1, 0, false, false})),
            rightVelPID(PID::PID({1, 0, 0, -1, 1, 0, false, false})),
      rightPosPID(PID::PID({1, 0, 0, -1, 1, 0, false, false})),
      rightCurPID(PID::PID({1, 0, 0, -1, 1, 0, false, false})),
      leftVel(Controller::Velocity(leftMotor, leftEncoder, std::nullopt, leftVelPID, POLOLUMAXDEGPERSEC,
                                    POLOLUMAXCURRENT, NC, NC)),
      leftPos(Controller::Position(leftMotor, leftEncoder, std::nullopt, leftPosPID, POLOLUMAXDEGPERSEC,
                                    POLOLUMAXCURRENT, NC, NC)),
      leftOpen(Controller::OpenLoop(leftMotor, leftEncoder, std::nullopt, POLOLUMAXDEGPERSEC, POLOLUMAXCURRENT, NC,
                                     NC)),
        rightVel(Controller::Velocity(rightMotor, rightEncoder, std::nullopt, rightVelPID, POLOLUMAXDEGPERSEC,
                                     POLOLUMAXCURRENT, NC, NC))),
      rightPos(Controller::Position(rightMotor, rightEncoder, std::nullopt, rightPosPID, POLOLUMAXDEGPERSEC,
                                     POLOLUMAXCURRENT, NC, NC)),
      rightOpen(Controller::OpenLoop(rightMotor, rightEncoder, std::nullopt, POLOLUMAXDEGPERSEC, POLOLUMAXCURRENT, NC,
                                      NC)),
      leftManager(Controller::ActuatorControllerManager(leftLut, HWBRIDGE::CONTROL::Mode::OPEN_LOOP),
            rightManager(Controller::ActuatorControllerManager(rightLut, HWBRIDGE::CONTROL::Mode::OPEN_LOOP)) {}
) {}

void periodic_1ms(CANInterface& can) override {
  // Determine new left wrist set point
  switch (Wrist::leftManager.getActiveControlMode()) {
    case HWBRIDGE::CONTROL::Mode::OPEN_LOOP:
    case HWBRIDGE::CONTROL::Mode::POSITION:
      can.getRXSignalValue(HWBRIDGE::CANID::ARM_SET_JOINT_POSITION, HWBRIDGE::CANSIGNAL::ARM_SET_LEFT_WRIST_POSITION,
                           leftWristSetPoint);
      leftWristSetPoint = RAD_TO_DEG(leftWristSetPoint);
      break;

    case HWBRIDGE::CONTROL::Mode::VELOCITY:
      can.getRXSignalValue(HWBRIDGE::CANID::ARM_SET_JOINT_ANGULAR_VELOCITY,
                           HWBRIDGE::CANSIGNAL::ARM_SET_LEFT_WRIST_ANGULAR_VELOCITY, leftWristSetPoint);
      leftWristSetPoint = RAD_TO_DEG(leftWristSetPoint);
      break;

    case HWBRIDGE::CONTROL::Mode::CURRENT:
      can.getRXSignalValue(HWBRIDGE::CANID::ARM_SET_JOINT_CURRENT, HWBRIDGE::CANSIGNAL::ARM_SET_LEFT_WRIST_CURRENT,
                           leftWristSetPoint);
      break;

    default:
      break;
  }

  // Determine new right wrist set point
  switch (Wrist::rightManager.getActiveControlMode()) {
    case HWBRIDGE::CONTROL::Mode::OPEN_LOOP:
    case HWBRIDGE::CONTROL::Mode::POSITION:
      can.getRXSignalValue(HWBRIDGE::CANID::ARM_SET_JOINT_POSITION, HWBRIDGE::CANSIGNAL::ARM_SET_RIGHT_WRIST_POSITION,
                           rightWristSetPoint);
      rightWristSetPoint = RAD_TO_DEG(rightWristSetPoint);
      break;

    case HWBRIDGE::CONTROL::Mode::VELOCITY:
      can.getRXSignalValue(HWBRIDGE::CANID::ARM_SET_JOINT_ANGULAR_VELOCITY,
                           HWBRIDGE::CANSIGNAL::ARM_SET_RIGHT_WRIST_ANGULAR_VELOCITY, rightWristSetPoint);
      rightWristSetPoint = RAD_TO_DEG(rightWristSetPoint);
      break;

    case HWBRIDGE::CONTROL::Mode::CURRENT:
      can.getRXSignalValue(HWBRIDGE::CANID::ARM_SET_JOINT_CURRENT, HWBRIDGE::CANSIGNAL::ARM_SET_RIGHT_WRIST_CURRENT,
                           rightWristSetPoint);
      break;

    default:
      break;
  }

  Wrist::leftManager.getActiveController()->setSetPoint(static_cast<float>(leftWristSetPoint));
  Wrist::rightManager.getActiveController()->setSetPoint(static_cast<float>(rightWristSetPoint));
  Wrist::leftManager.getActiveController()->update();
  Wrist::rightManager.getActiveController()->update();
  can.setTXSignalValue(HWBRIDGE::CANID::ARM_REPORT_JOINT_POSITION, HWBRIDGE::CANSIGNAL::ARM_REPORT_LEFT_WRIST_POSITION,
                       DEG_TO_RAD(Wrist::leftManager.getActiveController()->reportAngleDeg()));

  can.setTXSignalValue(HWBRIDGE::CANID::ARM_REPORT_JOINT_POSITION, HWBRIDGE::CANSIGNAL::ARM_REPORT_RIGHT_WRIST_POSITION,
                       DEG_TO_RAD(Wrist::rightManager.getActiveController()->reportAngleDeg()));

  can.setTXSignalValue(HWBRIDGE::CANID::ARM_REPORT_JOINT_ANGULAR_VELOCITY,
                       HWBRIDGE::CANSIGNAL::ARM_REPORT_LEFT_WRIST_ANGULAR_VELOCITY,
                       DEG_TO_RAD(Wrist::leftManager.getActiveController()->reportAngularVelocityDegPerSec()));

  can.setTXSignalValue(HWBRIDGE::CANID::ARM_REPORT_JOINT_ANGULAR_VELOCITY,
                       HWBRIDGE::CANSIGNAL::ARM_REPORT_RIGHT_WRIST_ANGULAR_VELOCITY,
                       DEG_TO_RAD(Wrist::rightManager.getActiveController()->reportAngularVelocityDegPerSec()));
}
