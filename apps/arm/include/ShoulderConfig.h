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

namespace Shoulder {

namespace Internal {

static Encoder::AEAT6012 encoder({ENC_PWM_SHLDR, 0});

static Actuator::DCMotor motor(MTR_PWM_SHLDR, MTR_DIR_SHLDR, false);

static Sensor::CurrentSensor currentSensor;

static PID::PID velPID({1, 0, 0, -1, 1, 0, false, false});
static PID::PID posPID({1, 0, 0, -1, 1, 0, false, false});
static PID::PID curPID({1, 0, 0, -1, 1, 0, false, false});

constexpr uint8_t PA_O4maxCurrent = 6;
constexpr uint8_t PA_O4maxRPM     = 0;  // younes todo figure out max rpm of shouder

// younes todo figure limit switches
static Controller::Velocity vel(&motor, &encoder, &currentSensor, &velPID, PA_O4maxRPM, PA_O4maxRPM, NC, NC);
static Controller::Position pos(&motor, &encoder, &currentSensor, &posPID, PA_O4maxRPM, PA_O4maxRPM, NC, NC);
static Controller::Current cur(&motor, &encoder, &currentSensor, &curPID, PA_O4maxRPM, PA_O4maxRPM, NC,
                               NC);  // younes todo figure out RPM of shuolder
static Controller::OpenLoop open(&motor, &encoder, &currentSensor, PA_O4maxRPM, PA_O4maxRPM, NC, NC);

static const LookupTable::LookupTable<HWBRIDGE::CONTROL::Mode, Controller::ActuatorController *> lut = {
    {HWBRIDGE::CONTROL::Mode::Velocity, &vel},
    {HWBRIDGE::CONTROL::Mode::Position, &pos},
    {HWBRIDGE::CONTROL::Mode::Current, &cur},
    {HWBRIDGE::CONTROL::Mode::OpenLoop, &open}};
}  // namespace Internal

static Controller::ActuatorControllerManager manager(Internal::lut, HWBRIDGE::CONTROL::Mode::OpenLoop);

}  // namespace Shoulder
