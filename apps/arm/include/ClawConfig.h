#pragma once

#include "ActuatorControllerManager.h"
#include "Current.h"
#include "DCMotor.h"
#include "LookupTable.h"
#include "OpenLoop.h"
#include "PID.h"
#include "Pololu37D.h"
#include "Position.h"
#include "Velocity.h"
#include "hw_bridge.h"

namespace Claw {

namespace Internal {

static Encoder::Pololu37D encoder({ENC_QUAD_CLAW_A, ENC_QUAD_CLAW_B, NC, 0, GPIO::QEI::Encoding::X2_ENCODING});

static Actuator::DCMotor motor(MTR_PWM_TRNTBL, MTR_DIR_TRNTBL, false);

static DigitalIn lowerLimit(LIM_CLAW_OPEN);

static Sensor::CurrentSensor currentSensor;

static PID::PID velPID({1, 0, 0, -1, 1, 0, false, false});
static PID::PID posPID({1, 0, 0, -1, 1, 0, false, false});
static PID::PID curPID({1, 0, 0, -1, 1, 0, false, false});

constexpr float pololuMaxCurrent = 3;
constexpr float pololuMaxRPM     = 1680;

// younes todo figure limit switches
static Controller::Velocity vel(&motor, &encoder, &currentSensor, &velPID, pololuMaxRPM, pololuMaxCurrent, &lowerLimit,
                                std::nullopt);
static Controller::Position pos(&motor, &encoder, &currentSensor, &posPID, pololuMaxRPM, pololuMaxCurrent, &lowerLimit,
                                std::nullopt);
static Controller::Current cur(&motor, &encoder, &currentSensor, &curPID, pololuMaxRPM, pololuMaxCurrent, &lowerLimit,
                               std::nullopt);
static Controller::OpenLoop open(&motor, &encoder, &currentSensor, pololuMaxRPM, pololuMaxCurrent, &lowerLimit,
                                 std::nullopt);

static const LookupTable::LookupTable<HWBRIDGE::CONTROL::Mode, Controller::ActuatorController *> lut = {
    {HWBRIDGE::CONTROL::Mode::Velocity, &vel},
    {HWBRIDGE::CONTROL::Mode::Position, &pos},
    {HWBRIDGE::CONTROL::Mode::Current, &cur},
    {HWBRIDGE::CONTROL::Mode::OpenLoop, &open}};
}  // namespace Internal

static Controller::ActuatorControllerManager manager(Internal::lut, HWBRIDGE::CONTROL::Mode::OpenLoop);

}  // namespace Claw
