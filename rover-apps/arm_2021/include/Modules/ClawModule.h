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

class ClawModule final : public Module {
 public:
  ElbowModule();
  HWBRIDGE::CANSignalValue_t clawSetPoint;

  void periodic_1s(void) override;
  void periodic_10s(void) override;
  void periodic_100ms(void) override;
  void periodic_10ms(void) override;
  void periodic_1ms(CANInterface& can) override;

  Encoder::Quadrature64CPR encoder;
  Actuator::DCMotor motor;

  // static Sensor::CurrentSensor currentSensor(CLAW_CRNT_SNS_SPI_CLK, CLAW_CRNT_SNS_SPI_MISO, CLAW_CRNT_SNS_SPI_CS);
  // TODO: Add once current sensor driver works

  PID::PID velPID;
  PID::PID posPID;
  PID::PID curPID;

  constexpr float POLOLUMAXCURRENT = 3;
  constexpr float POLOLUMADEGPERSEC =
      std::numeric_limits<float>::infinity();  // TODO: figure out MAXDEGPERSEC of motors (1680?);

  Controller::Velocity vel;
  Controller::Position pos;
  // static Controller::Current cur(motor, encoder, std::nullopt, curPID, POLOLUMADEGPERSEC, POLOLUMAXCURRENT,
  // LIM_CLAW_OPEN, NC);
  Controller::OpenLoop open;

  const Controller::ControlMap lut = {{HWBRIDGE::CONTROL::Mode::VELOCITY, &vel},
                                      {HWBRIDGE::CONTROL::Mode::POSITION, &pos},
                                      // {HWBRIDGE::CONTROL::Mode::CURRENT, &cur},
                                      {HWBRIDGE::CONTROL::Mode::OPEN_LOOP, &open}};

  Controller::ActuatorControllerManager manager;

 private:
}