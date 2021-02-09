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

constexpr uint8_t assunMaxCurrent = 0;  // younes todo figure out max rpm and max current of elbow
constexpr uint16_t assunMaxRPM    = 0;

// younes todo figure limit switches
static Controller::Velocity vel(&motor, &encoder, &currentSensor, &velPID, assunMaxRPM, assunMaxCurrent, LIM_TRNTBL_LHS,
                                LIM_TRNTBL_RHS);
static Controller::Position pos(&motor, &encoder, &currentSensor, &posPID, assunMaxRPM, assunMaxCurrent, LIM_TRNTBL_LHS,
                                LIM_TRNTBL_RHS);
static Controller::Current cur(&motor, &encoder, &currentSensor, &curPID, assunMaxRPM, assunMaxCurrent, LIM_TRNTBL_LHS,
                               LIM_TRNTBL_RHS);
static Controller::OpenLoop open(&motor, &encoder, &currentSensor, assunMaxRPM, assunMaxCurrent, LIM_TRNTBL_LHS,
                                 LIM_TRNTBL_RHS);

static const LookupTable::LookupTable<HWBRIDGE::CONTROL::Mode, Controller::ActuatorController *> lut = {
    {HWBRIDGE::CONTROL::Mode::Velocity, &vel},
    {HWBRIDGE::CONTROL::Mode::Position, &pos},
    {HWBRIDGE::CONTROL::Mode::Current, &cur},
    {HWBRIDGE::CONTROL::Mode::OpenLoop, &open}};
}  // namespace Internal

static Controller::ActuatorControllerManager manager(Internal::lut, HWBRIDGE::CONTROL::Mode::OpenLoop);

}  // namespace Elbow
