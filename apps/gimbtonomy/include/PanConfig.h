#pragma once

#include <limits>

#include "ActuatorControllerManager.h"
#include "ContServo.h"
#include "MAE3.h"
#include "OpenLoop.h"
#include "Position.h"

namespace Pan {

namespace Internal {

constexpr auto maxRPM     = 38;  // = 228 deg per sec younes todo
constexpr auto maxCurrent = std::numeric_limits<float>::infinity();
constexpr auto maxPulse = 2.1ms, minPulse = 0.9ms;

static PID::PID pid({1, 0, 0, -maxRPM, maxRPM, 0, false, false});

static Actuator::ContServo servo(SRVO_PWM_CR, maxRPM, maxPulse, minPulse);
static Encoder::MAE3 encoder({ENC_PWM_GIMB, 0});

static Controller::Position pos(&servo, &encoder, std::nullopt, &pid, maxRPM, 0, NC, NC);
static Controller::Position vel(&servo, &encoder, std::nullopt, &pid, maxRPM, 0, NC, NC);
static Controller::OpenLoop open(&servo, &encoder, std::nullopt, maxRPM, 0, NC, NC);

// younes todo everything should be in degpersec not rpm since encoder read latter. do motors do internal checking with
// rpm?

static const LookupTable::LookupTable<HWBRIDGE::CONTROL::Mode, Controller::ActuatorController *> lut = {
    {HWBRIDGE::CONTROL::Mode::Position, &pos},
    {HWBRIDGE::CONTROL::Mode::OpenLoop, &open},
    {HWBRIDGE::CONTROL::Mode::Velocity, &vel}};

}  // namespace Internal
static Controller::ActuatorControllerManager manager(Internal::lut, HWBRIDGE::CONTROL::Mode::OpenLoop);
}  // namespace Pan