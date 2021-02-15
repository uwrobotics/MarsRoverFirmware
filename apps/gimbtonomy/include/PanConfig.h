#pragma once

#include <limits>

#include "ActuatorControllerManager.h"
#include "ContServo.h"
#include "MAE3.h"
#include "OpenLoop.h"
#include "Position.h"

namespace Pan {

namespace Internal {

constexpr auto maxDegPerSec =
    std::numeric_limits<float>::infinity();  // TODO: figure out maxDegPerSec of motors (228?);
constexpr auto maxCurrent = std::numeric_limits<float>::infinity();
constexpr auto maxPulse = 2.1ms, minPulse = 0.9ms;  // Orson TODO: Confirm these 2 values with EE and ME

static PID::PID pid({1, 0, 0, -maxDegPerSec, maxDegPerSec, 0, false, false});

static Actuator::ContServo servo(SRVO_PWM_CR, maxDegPerSec, maxPulse, minPulse);
static Encoder::MAE3 encoder({NC, 0});  // Orson TODO: figure out if Pan Limit Servo on gimbal uses MAE3. (i
                                        // think it uses AEAT6012) also use the right pins

static Controller::Position pos(
    servo, encoder, std::nullopt, pid, maxDegPerSec, maxCurrent, NC,
    NC);  // Orson TODO: confirm with ee and me that there are no limit switches if there are add them to these objects
static Controller::Position vel(servo, encoder, std::nullopt, pid, maxDegPerSec, maxCurrent, NC, NC);
static Controller::OpenLoop open(servo, encoder, std::nullopt, maxDegPerSec, maxCurrent, NC, NC);

static const Controller::ControlMap lut = {{HWBRIDGE::CONTROL::Mode::Position, &pos},
                                           {HWBRIDGE::CONTROL::Mode::OpenLoop, &open},
                                           {HWBRIDGE::CONTROL::Mode::Velocity, &vel}};

}  // namespace Internal
static Controller::ActuatorControllerManager manager(Internal::lut, HWBRIDGE::CONTROL::Mode::OpenLoop);
}  // namespace Pan