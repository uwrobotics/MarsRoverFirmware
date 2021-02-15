#pragma once

#include <numeric>

#include "ActuatorControllerManager.h"
#include "DCMotor.h"
#include "LookupTable.h"
#include "OpenLoop.h"
#include "Pololu37D.h"
#include "Position.h"
#include "Velocity.h"
#include "hw_bridge.h"

namespace Elevator {

namespace Internal {
static Actuator::DCMotor motor(MTR_PWM_2, MTR_DIR_2, false);

// Felix TODO: ensure that Elevator uses Pololu encoder
static Encoder::Pololu37D encoder({NC, NC, NC, 0});

static PID::PID velPID({1, 0, 0, -1, 1, 0, false, false});
static PID::PID posPID({1, 0, 0, -1, 1, 0, false, false});

constexpr float maxDegPerSec =
    std::numeric_limits<float>::infinity();  // TODO: figure out maxDegPerSec of motors (1197.482?)
constexpr float maxCurrent = std::numeric_limits<float>::infinity();  // since no current sensor

static Controller::Velocity vel(motor, encoder, std::nullopt, velPID, maxDegPerSec, maxCurrent, LIM_SW_3, LIM_SW_4);
static Controller::Position pos(motor, encoder, std::nullopt, posPID, maxDegPerSec, maxCurrent, LIM_SW_3, LIM_SW_4);
static Controller::OpenLoop open(motor, encoder, std::nullopt, maxDegPerSec, maxCurrent, LIM_SW_3, LIM_SW_4);

static const Controller::ControlMap lut = {{HWBRIDGE::CONTROL::Mode::Velocity, &vel},
                                           {HWBRIDGE::CONTROL::Mode::Position, &pos},
                                           {HWBRIDGE::CONTROL::Mode::OpenLoop, &open}};
}  // namespace Internal

static Controller::ActuatorControllerManager manager(Internal::lut, HWBRIDGE::CONTROL::Mode::OpenLoop);

}  // namespace Elevator