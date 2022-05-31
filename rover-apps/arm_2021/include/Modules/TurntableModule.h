#pragma once

#include "AEAT6012.h"
#include "ActuatorControllerManager.h"
#include "Current.h"
#include "DCMotor.h"
#include "LookupTable.h"
#include "Module.h"
#include "OpenLoop.h"
#include "PID.h"
#include "Position.h"
#include "Velocity.h"
#include "hw_bridge.h"
#include "mbed.h"

class TurntableModule final : public Module {
 public:
  TurntableModule();
  HWBRIDGE::CANSignalValue_t turntableSetPoint;

  void periodic_1s(void) override;
  void periodic_10s(void) override;
  void periodic_100ms(void) override;
  void periodic_10ms(void) override;
  void periodic_1ms(CANInterface& can) override;

  Encoder::AEAT6012 encoder;
  Actuator::DCMotor motor;

  // static Sensor::CurrentSensor currentSensor(TRNTBL_CRNT_SNS_SPI_CLK, TRNTBL_CRNT_SNS_SPI_MISO,
  // TRNTBL_CRNT_SNS_SPI_CS);
  // TODO: Add once current sensor driver works

  PID::PID velPID;
  PID::PID posPID;
  PID::PID curPID;

  constexpr uint8_t MAXCURRENT = 53;
  constexpr float MAXDEGPERSEC = std::numeric_limits<float>::infinity();
  // TODO: figure out MAXDEGPERSEC of motors (79080?)

  Controller::Velocity vel;
  Controller::Position pos;
  // static Controller::Current cur(motor, encoder, std::nullopt, curPID, MAXDEGPERSEC, MAXCURRENT, LIM_TRNTBL_LHS,
  //                               LIM_TRNTBL_RHS);
  Controller::OpenLoop open;

  const Controller::ControlMap lut = {{HWBRIDGE::CONTROL::Mode::VELOCITY, &vel},
                                      {HWBRIDGE::CONTROL::Mode::POSITION, &pos},
                                      // {HWBRIDGE::CONTROL::Mode::CURRENT, &cur},
                                      {HWBRIDGE::CONTROL::Mode::OPEN_LOOP, &open}};

  Controller::ActuatorControllerManager manager;

 private:
}