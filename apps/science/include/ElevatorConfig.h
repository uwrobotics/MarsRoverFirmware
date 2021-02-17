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

// channela, channelb, index, offset deg, qei encoding
static Encoder::Pololu37D encoder({ENC_A_LIFT, ENC_B_LIFT, NC, 0});

static PID::PID velPID({1, 0, 0, -1, 1, 0, false, false});
static PID::PID posPID({1, 0, 0, -1, 1, 0, false, false});

constexpr float MAX_DEG_PRE_SEC = 1197;                                    // 20.9 RAD/s
constexpr float MAX_CURRENT     = std::numeric_limits<float>::infinity();  // since no current sensor

static Controller::Velocity vel(motor, encoder, std::nullopt, velPID, MAX_DEG_PRE_SEC, MAX_CURRENT, LIM_SW_ELVTR_DN,
                                LIM_SW_ELVTR_UP);
static Controller::Position pos(motor, encoder, std::nullopt, posPID, MAX_DEG_PRE_SEC, MAX_CURRENT, LIM_SW_ELVTR_DN,
                                LIM_SW_ELVTR_UP);
static Controller::OpenLoop open(motor, encoder, std::nullopt, MAX_DEG_PRE_SEC, MAX_CURRENT, LIM_SW_ELVTR_DN,
                                 LIM_SW_ELVTR_UP);

static const Controller::ControlMap lut = {{HWBRIDGE::CONTROL::Mode::Velocity, &vel},
                                           {HWBRIDGE::CONTROL::Mode::Position, &pos},
                                           {HWBRIDGE::CONTROL::Mode::OpenLoop, &open}};
}  // namespace Internal

static Controller::ActuatorControllerManager manager(Internal::lut, HWBRIDGE::CONTROL::Mode::OpenLoop);

}  // namespace Elevator
