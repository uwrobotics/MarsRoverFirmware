#pragma once

#include "AEAT6012.h"
#include "ActuatorControllerManager.h"
#include "ContServo.h"
#include "OpenLoop.h"
#include "Position.h"

namespace Pan {

// Servo Model: HSR-1425CR

namespace Internal {

constexpr auto maxDegPerSec = 267;
constexpr auto maxCurrent   = 0.64;
constexpr auto maxPulse = 2.1ms, minPulse = 0.9ms;

static PID::PID pid({1, 0, 0, -maxDegPerSec, maxDegPerSec, 0, false, false});

static Actuator::ContServo servo(SRVO_PWM_YAW, maxDegPerSec, maxPulse, minPulse);
static Encoder::AEAT6012 encoder({SPI1_SCK, SPI1_MISO, SPI1_CS, 0});

static Controller::Position pos(&servo, &encoder, std::nullopt, &pid, maxDegPerSec, maxCurrent, NC, NC);
static Controller::Position vel(&servo, &encoder, std::nullopt, &pid, maxDegPerSec, maxCurrent, NC, NC);
static Controller::OpenLoop open(&servo, &encoder, std::nullopt, maxDegPerSec, maxCurrent, NC, NC);

static const LookupTable::LookupTable<HWBRIDGE::CONTROL::Mode, Controller::ActuatorController *> lut = {
    {HWBRIDGE::CONTROL::Mode::Position, &pos},
    {HWBRIDGE::CONTROL::Mode::OpenLoop, &open},
    {HWBRIDGE::CONTROL::Mode::Velocity, &vel}};

}  // namespace Internal
static Controller::ActuatorControllerManager manager(Internal::lut, HWBRIDGE::CONTROL::Mode::OpenLoop);
}  // namespace Pan
