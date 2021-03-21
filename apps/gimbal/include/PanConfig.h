#pragma once

#include "AEAT6012.h"
#include "ActuatorControllerManager.h"
#include "ContServo.h"
#include "OpenLoop.h"
#include "Position.h"
#include "Velocity.h"

namespace Pan {

/*
• Pan: HS-1425 CR (https://www.robotshop.com/en/hitec-hsr-1425cr-continuous-rotation-servo.html)
*/

namespace Internal {

constexpr auto MAX_DEG_PER_SEC = 267;
constexpr auto MAX_CURRENT     = 0.64;
constexpr auto MAX_PULSE = 2.1ms, MIN_PULSE = 0.9ms;

static PID::PID pid({1, 0, 0, -MAX_DEG_PER_SEC, MAX_DEG_PER_SEC, 0, false, false});

static Actuator::ContServo servo(SRVO_PWM_YAW, MAX_DEG_PER_SEC, MAX_PULSE, MIN_PULSE);
static Encoder::AEAT6012 encoder({PAN_ENC_SPI_SCK, PAN_ENC_SPI_MISO, PAN_ENC_SPI_CS, 0});

static Controller::Position pos(servo, encoder, std::nullopt, pid, MAX_DEG_PER_SEC, MAX_CURRENT, NC, NC);
static Controller::Velocity vel(servo, encoder, std::nullopt, pid, MAX_DEG_PER_SEC, MAX_CURRENT, NC, NC);
static Controller::OpenLoop open(servo, encoder, std::nullopt, MAX_DEG_PER_SEC, MAX_CURRENT, NC, NC);

static const Controller::ControlMap lut = {{HWBRIDGE::CONTROL::Mode::POSITION, &pos},
                                           {HWBRIDGE::CONTROL::Mode::OPEN_LOOP, &open},
                                           {HWBRIDGE::CONTROL::Mode::VELOCITY, &vel}};

}  // namespace Internal
static Controller::ActuatorControllerManager manager(Internal::lut, HWBRIDGE::CONTROL::Mode::OPEN_LOOP);
}  // namespace Pan
