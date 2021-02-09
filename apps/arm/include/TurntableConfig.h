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

static Encoder::AEAT6012 encoder({ENC_PWM_TRNTBL, 0});

static Actuator::DCMotor motor(MTR_PWM_TRNTBL, MTR_DIR_TRNTBL, false);

static Sensor::CurrentSensor currentSensor;

static PID::PID velPID({1, 0, 0, -1, 1, 0, false, false});
static PID::PID posPID({1, 0, 0, -1, 1, 0, false, false});
static PID::PID curPID({1, 0, 0, -1, 1, 0, false, false});

constexpr uint8_t maxCurrent    = 53;
constexpr uint32_t maxDegPerSec = 79080;

static Controller::Velocity vel(&motor, &encoder, &currentSensor, &velPID, maxDegPerSec, maxCurrent, LIM_TRNTBL_LHS,
                                LIM_TRNTBL_RHS);
static Controller::Position pos(&motor, &encoder, &currentSensor, &posPID, maxDegPerSec, maxCurrent, LIM_TRNTBL_LHS,
                                LIM_TRNTBL_RHS);
static Controller::Current cur(&motor, &encoder, &currentSensor, &curPID, maxDegPerSec, maxCurrent, LIM_TRNTBL_LHS,
                               LIM_TRNTBL_RHS);
static Controller::OpenLoop open(&motor, &encoder, &currentSensor, maxDegPerSec, maxCurrent, LIM_TRNTBL_LHS,
                                 LIM_TRNTBL_RHS);

static const LookupTable::LookupTable<HWBRIDGE::CONTROL::Mode, Controller::ActuatorController *> lut = {
    {HWBRIDGE::CONTROL::Mode::Velocity, &vel},
    {HWBRIDGE::CONTROL::Mode::Position, &pos},
    {HWBRIDGE::CONTROL::Mode::Current, &cur},
    {HWBRIDGE::CONTROL::Mode::OpenLoop, &open}};
}  // namespace Internal

static Controller::ActuatorControllerManager manager(Internal::lut, HWBRIDGE::CONTROL::Mode::OpenLoop);

}  // namespace Turntable
