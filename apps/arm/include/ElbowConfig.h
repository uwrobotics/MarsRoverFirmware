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

namespace Elbow {

namespace Internal {

static Encoder::AEAT6012 encoder({ENC_PWM_ELBW, 0});

static Actuator::DCMotor motor(MTR_PWM_ELBW, MTR_DIR_ELBW, false);

static Sensor::CurrentSensor currentSensor;

static PID::PID velPID({1, 0, 0, -1, 1, 0, false, false});
static PID::PID posPID({1, 0, 0, -1, 1, 0, false, false});
static PID::PID curPID({1, 0, 0, -1, 1, 0, false, false});

constexpr float assunMaxCurrent   = 25.263;
constexpr float assunMaxDegPerSec = 35580;

static Controller::Velocity vel(&motor, &encoder, &currentSensor, &velPID, assunMaxDegPerSec, assunMaxCurrent,
                                LIM_ELBW_DN, LIM_ELBW_UP);
static Controller::Position pos(&motor, &encoder, &currentSensor, &posPID, assunMaxDegPerSec, assunMaxCurrent,
                                LIM_ELBW_DN, LIM_ELBW_UP);
static Controller::Current cur(&motor, &encoder, &currentSensor, &curPID, assunMaxDegPerSec, assunMaxCurrent,
                               LIM_ELBW_DN, LIM_ELBW_UP);
static Controller::OpenLoop open(&motor, &encoder, &currentSensor, assunMaxDegPerSec, assunMaxCurrent, LIM_ELBW_DN,
                                 LIM_ELBW_UP);

static const LookupTable::LookupTable<HWBRIDGE::CONTROL::Mode, Controller::ActuatorController *> lut = {
    {HWBRIDGE::CONTROL::Mode::Velocity, &vel},
    {HWBRIDGE::CONTROL::Mode::Position, &pos},
    {HWBRIDGE::CONTROL::Mode::Current, &cur},
    {HWBRIDGE::CONTROL::Mode::OpenLoop, &open}};
}  // namespace Internal

static Controller::ActuatorControllerManager manager(Internal::lut, HWBRIDGE::CONTROL::Mode::OpenLoop);

}  // namespace Elbow
