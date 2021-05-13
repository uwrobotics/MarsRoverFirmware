#pragma once

#include <numeric>

#include "ActuatorControllerManager.h"
#include "Current.h"
#include "DCMotor.h"
#include "LookupTable.h"
#include "OpenLoop.h"
#include "PID.h"
#include "Pololu37D.h"
#include "Position.h"
#include "Velocity.h"
#include "hw_bridge.h"
#include "units.h"

using namespace units::current;

namespace Claw {

namespace Internal {

static Encoder::Pololu37D encoder({ENC_QUAD_CLAW_A, ENC_QUAD_CLAW_B, NC, 0, GPIO::QEI::Encoding::X2_ENCODING});

static Actuator::DCMotor motor(MTR_PWM_TRNTBL, MTR_DIR_TRNTBL, false);

// static Sensor::CurrentSensor currentSensor(CLAW_CRNT_SNS_SPI_CLK, CLAW_CRNT_SNS_SPI_MISO, CLAW_CRNT_SNS_SPI_CS);
// TODO: Add once current sensor driver works

static PID::PID velPID({1, 0, 0, -1, 1, 0, false, false});
static PID::PID posPID({1, 0, 0, -1, 1, 0, false, false});
static PID::PID curPID({1, 0, 0, -1, 1, 0, false, false});

constexpr ampere_t POLOLUMAXCURRENT = ampere_t(3);
constexpr float POLOLUMADEGPERSEC =
    std::numeric_limits<float>::infinity();  // TODO: figure out MAXDEGPERSEC of motors (1680?);

static Controller::Velocity vel(motor, encoder, std::nullopt, velPID, POLOLUMADEGPERSEC, POLOLUMAXCURRENT,
                                LIM_CLAW_OPEN, NC);
static Controller::Position pos(motor, encoder, std::nullopt, posPID, POLOLUMADEGPERSEC, POLOLUMAXCURRENT,
                                LIM_CLAW_OPEN, NC);
static Controller::Current cur(motor, encoder, std::nullopt, curPID, POLOLUMADEGPERSEC, POLOLUMAXCURRENT, LIM_CLAW_OPEN,
                               NC);
static Controller::OpenLoop open(motor, encoder, std::nullopt, POLOLUMADEGPERSEC, POLOLUMAXCURRENT, LIM_CLAW_OPEN, NC);

static const Controller::ControlMap lut = {{HWBRIDGE::CONTROL::Mode::VELOCITY, &vel},
                                           {HWBRIDGE::CONTROL::Mode::POSITION, &pos},
                                           {HWBRIDGE::CONTROL::Mode::CURRENT, &cur},
                                           {HWBRIDGE::CONTROL::Mode::OPEN_LOOP, &open}};
}  // namespace Internal

static Controller::ActuatorControllerManager manager(Internal::lut, HWBRIDGE::CONTROL::Mode::OPEN_LOOP);

}  // namespace Claw
