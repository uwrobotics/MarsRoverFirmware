#pragma once

#include <numeric>

#include "ActuatorControllerManager.h"
#include "Current.h"
#include "DCMotor.h"
#include "LookupTable.h"
#include "OpenLoop.h"
#include "PID.h"
#include "Position.h"
#include "Quadrature64CPR.h"
#include "Velocity.h"
#include "hw_bridge.h"

class WristModule final : public Module {
 public:
  WristModule();
  HWBRIDGE::CANSignalValue_t leftWristSetPoint  = 0;
  HWBRIDGE::CANSignalValue_t rightWristSetPoint = 0;

  void periodic_1s(void) override;
  void periodic_10s(void) override;
  void periodic_100ms(void) override;
  void periodic_10ms(void) override;
  void periodic_1ms(CANInterface& can) override;

  Encoder::Quadrature64CPR leftEncoder;
  Encoder::Quadrature64CPR rightEncoder;

  Actuator::DCMotor leftMotor;
  Actuator::DCMotor rightMotor;

  /* TODO: Can we use wrist limit switches if motors are individually addressable */

  // static Sensor::CurrentSensor leftCurrentSensor(WRSTL_CRNT_SNS_SPI_CLK, WRSTL_CRNT_SNS_SPI_MISO,
  // WRSTL_CRNT_SNS_SPI_CS),
  //     rightCurrentSensor(WRSTR_CRNT_SNS_SPI_CLK, WRSTR_CRNT_SNS_SPI_MISO, WRSTR_CRNT_SNS_SPI_CS);
  // TODO: Add once current sensor driver works

  PID::PID leftVelPID({1, 0, 0, -1, 1, 0, false, false});
  PID::PID leftPosPID({1, 0, 0, -1, 1, 0, false, false});
  PID::PID leftCurPID({1, 0, 0, -1, 1, 0, false, false});

  PID::PID rightVelPID({1, 0, 0, -1, 1, 0, false, false});
  PID::PID rightPosPID({1, 0, 0, -1, 1, 0, false, false});
  PID::PID rightCurPID({1, 0, 0, -1, 1, 0, false, false});

  constexpr uint8_t POLOLUMAXCURRENT = 3;
  constexpr float POLOLUMAXDEGPERSEC = std::numeric_limits<float>::infinity();
  // TODO: figure out MAXDEGPERSEC of motors (1680?)

  Controller::Velocity leftVel;
  Controller::Position leftPos;
  // Controller::Current leftCur(leftMotor, leftEncoder, std::nullopt, leftCurPID, POLOLUMAXDEGPERSEC,
  //
  Controller::OpenLoop leftOpen;

  Controller::Velocity rightVel;
  Controller::Position rightPos;
  // Controller::Current rightCur(rightMotor, rightEncoder, std::nullopt, rightCurPID, POLOLUMAXDEGPERSEC,
  //
  Controller::OpenLoop rightOpen;

  const Controller::ControlMap leftLut  = {{HWBRIDGE::CONTROL::Mode::VELOCITY, &leftVel},
                                          {HWBRIDGE::CONTROL::Mode::POSITION, &leftPos},
                                          // {HWBRIDGE::CONTROL::Mode::CURRENT, &leftCur},
                                          {HWBRIDGE::CONTROL::Mode::OPEN_LOOP, &leftOpen}};
  const Controller::ControlMap rightLut = {{HWBRIDGE::CONTROL::Mode::VELOCITY, &rightVel},
                                           {HWBRIDGE::CONTROL::Mode::POSITION, &rightPos},
                                           // {HWBRIDGE::CONTROL::Mode::CURRENT, &rightCur},
                                           {HWBRIDGE::CONTROL::Mode::OPEN_LOOP, &rightOpen}};
  Controller::ActuatorControllerManager leftManager;
  Controller::ActuatorControllerManager rightManager;

 private:
}