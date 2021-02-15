#pragma once

#include <numeric>

#include "AEAT6012.h"
#include "ActuatorControllerManager.h"
#include "DCMotor.h"
#include "LookupTable.h"
#include "OpenLoop.h"
#include "Position.h"
#include "Velocity.h"
#include "hw_bridge.h"

namespace Centrifuge {

namespace Internal {
static Actuator::DCMotor motor(MTR_PWM_1, MTR_DIR_1, false);

// Felix TODO: ensure that Centrifuge uses AEAT6012 encoder. If not please correct
static Encoder::AEAT6012 encoder({NC, NC, NC, 0});  // Felix TODO: Fill in correct pins for Centrifuge

static PID::PID velPID({1, 0, 0, -1, 1, 0, false, false});
static PID::PID posPID({1, 0, 0, -1, 1, 0, false, false});

constexpr float MAXDEGPERSEC =
    std::numeric_limits<float>::infinity();                           // TODO: figure out MAXDEGPERSEC of motors (601?)
constexpr float MAXCURRENT = std::numeric_limits<float>::infinity();  // since no current sensor

static Controller::Position pos(motor, encoder, std::nullopt, posPID, MAXDEGPERSEC, MAXCURRENT, LIM_SW_1, LIM_SW_2);
static Controller::OpenLoop open(motor, encoder, std::nullopt, MAXDEGPERSEC, MAXCURRENT, LIM_SW_1, LIM_SW_2);

static const Controller::ControlMap lut = {{HWBRIDGE::CONTROL::Mode::Position, &pos},
                                           {HWBRIDGE::CONTROL::Mode::OpenLoop, &open}};
}  // namespace Internal

static Controller::ActuatorControllerManager manager(Internal::lut, HWBRIDGE::CONTROL::Mode::OpenLoop);

}  // namespace Centrifuge
