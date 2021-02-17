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
static Actuator::DCMotor motor(MTR_PWM_CENTFGE, MTR_DIR_CENTFGE, false);

// clk, miso, cs
static Encoder::AEAT6012 encoder({ENC_SCK_CENTFGE, ENC_MISO_CENTFGE, ENC_CS_CENTFGE, 0});

static PID::PID velPID({1, 0, 0, -1, 1, 0, false, false});
static PID::PID posPID({1, 0, 0, -1, 1, 0, false, false});

constexpr float MAX_DEG_PER_SEC = 601;                                     // 10.5 RAD/s
constexpr float MAX_CURRENT     = std::numeric_limits<float>::infinity();  // since no current sensor

static Controller::Position pos(motor, encoder, std::nullopt, posPID, MAX_DEG_PER_SEC, MAX_CURRENT, LIM_SW_CENTFGE_DN,
                                LIM_SW_CENTFGE_UP);
static Controller::OpenLoop open(motor, encoder, std::nullopt, MAX_DEG_PER_SEC, MAX_CURRENT, LIM_SW_CENTFGE_DN,
                                 LIM_SW_CENTFGE_UP);

static const Controller::ControlMap lut = {{HWBRIDGE::CONTROL::Mode::Position, &pos},
                                           {HWBRIDGE::CONTROL::Mode::OpenLoop, &open}};
}  // namespace Internal

static Controller::ActuatorControllerManager manager(Internal::lut, HWBRIDGE::CONTROL::Mode::OpenLoop);

}  // namespace Centrifuge
