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
static Actuator::DCMotor motor(MTR_PWM_CENTRIFUGE, MTR_DIR_CENTRIFUGE, false);

// clk, miso, cs
static Encoder::AEAT6012 encoder({CENTRIFUGE_ENC_SCK, CENTRIFUGE_ENC_MISO, CENTRIFUGE_ENC_CS, 0});

static PID::PID velPID({1, 0, 0, -1, 1, 0, false, false});
static PID::PID posPID({1, 0, 0, -1, 1, 0, false, false});

constexpr float maxDegPerSec = 601;                                     // 10.5 RAD/s
constexpr float maxCurrent   = std::numeric_limits<float>::infinity();  // since no current sensor

static Controller::Position pos(&motor, &encoder, std::nullopt, &posPID, maxDegPerSec, maxCurrent, CENTRIFUGE_SW_1,
                                CENTRIFUGE_SW_2);
static Controller::OpenLoop open(&motor, &encoder, std::nullopt, maxDegPerSec, maxCurrent, CENTRIFUGE_SW_1,
                                 CENTRIFUGE_SW_2);

static const LookupTable::LookupTable<HWBRIDGE::CONTROL::Mode, Controller::ActuatorController *> lut = {
    {HWBRIDGE::CONTROL::Mode::Position, &pos}, {HWBRIDGE::CONTROL::Mode::OpenLoop, &open}};
}  // namespace Internal

static Controller::ActuatorControllerManager manager(Internal::lut, HWBRIDGE::CONTROL::Mode::OpenLoop);

}  // namespace Centrifuge
