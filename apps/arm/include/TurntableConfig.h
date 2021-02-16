#pragma once

#include "AEAT6012.h"
#include "ActuatorControllerManager.h"
#include "Current.h"
#include "DCMotor.h"
#include "LookupTable.h"
#include "OpenLoop.h"
#include "PID.h"
#include "Position.h"
#include "Velocity.h"
#include "hw_bridge.h"

namespace Turntable {

namespace Internal {

static Encoder::AEAT6012 encoder({TRNTBL_ENC_SPI_CLK, TRNTBL_ENC_SPI_MISO, NC, 0});

static Actuator::DCMotor motor(MTR_PWM_TRNTBL, MTR_DIR_TRNTBL, false);

// static Sensor::CurrentSensor currentSensor(TRNTBL_CRNT_SNS_SPI_CLK, TRNTBL_CRNT_SNS_SPI_MISO,
// TRNTBL_CRNT_SNS_SPI_CS);
// TODO: Add once current sensor driver works

static PID::PID velPID({1, 0, 0, -1, 1, 0, false, false});
static PID::PID posPID({1, 0, 0, -1, 1, 0, false, false});
static PID::PID curPID({1, 0, 0, -1, 1, 0, false, false});

constexpr uint8_t MAXCURRENT = 53;
constexpr float MAXDEGPERSEC =
    std::numeric_limits<float>::infinity();  // TODO: figure out MAXDEGPERSEC of motors (79080?)

static Controller::Velocity vel(motor, encoder, std::nullopt, velPID, MAXDEGPERSEC, MAXCURRENT, LIM_TRNTBL_LHS,
                                LIM_TRNTBL_RHS);
static Controller::Position pos(motor, encoder, std::nullopt, posPID, MAXDEGPERSEC, MAXCURRENT, LIM_TRNTBL_LHS,
                                LIM_TRNTBL_RHS);
static Controller::Current cur(motor, encoder, std::nullopt, curPID, MAXDEGPERSEC, MAXCURRENT, LIM_TRNTBL_LHS,
                               LIM_TRNTBL_RHS);
static Controller::OpenLoop open(motor, encoder, std::nullopt, MAXDEGPERSEC, MAXCURRENT, LIM_TRNTBL_LHS,
                                 LIM_TRNTBL_RHS);

static const Controller::ControlMap lut = {{HWBRIDGE::CONTROL::Mode::Velocity, &vel},
                                           {HWBRIDGE::CONTROL::Mode::Position, &pos},
                                           {HWBRIDGE::CONTROL::Mode::Current, &cur},
                                           {HWBRIDGE::CONTROL::Mode::OpenLoop, &open}};
}  // namespace Internal

static Controller::ActuatorControllerManager manager(Internal::lut, HWBRIDGE::CONTROL::Mode::OpenLoop);

}  // namespace Turntable
