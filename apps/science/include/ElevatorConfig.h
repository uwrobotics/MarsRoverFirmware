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
static Actuator::DCMotor motor(MTR_PWM_ELVTR, MTR_DIR_ELVTR, false);

//channela, channelb, index, offset deg, qei encoding
static Encoder::Pololu37D encoder({LIFT_ENC_A, LIFT_ENC_B, NC, 0});

static PID::PID velPID({1, 0, 0, -1, 1, 0, false, false});
static PID::PID posPID({1, 0, 0, -1, 1, 0, false, false});

constexpr float maxDegPerSec = 1197;//20.9 RAD/s 
constexpr float maxCurrent = std::numeric_limits<float>::infinity();  // since no current sensor

static Controller::Velocity vel(&motor, &encoder, std::nullopt, &velPID, maxDegPerSec, maxCurrent, ELVTR_SW_3, ELVTR_SW_4);
static Controller::Position pos(&motor, &encoder, std::nullopt, &posPID, maxDegPerSec, maxCurrent, ELVTR_SW_3, ELVTR_SW_4);
static Controller::OpenLoop open(&motor, &encoder, std::nullopt, maxDegPerSec, maxCurrent, ELVTR_SW_3, ELVTR_SW_4);

static const LookupTable::LookupTable<HWBRIDGE::CONTROL::Mode, Controller::ActuatorController *> lut = {
    {HWBRIDGE::CONTROL::Mode::Velocity, &vel},
    {HWBRIDGE::CONTROL::Mode::Position, &pos},
    {HWBRIDGE::CONTROL::Mode::OpenLoop, &open}};
}  // namespace Internal

static Controller::ActuatorControllerManager manager(Internal::lut, HWBRIDGE::CONTROL::Mode::OpenLoop);

}  // namespace Elevator
