#pragma once

#include <numeric>

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

static Encoder::AEAT6012 encoder({SHLDR_ENC_SPI_CLK, SHLDR_ENC_SPI_MISO, SHLDR_ENC_SPI_CS, 0});

static Actuator::DCMotor motor(MTR_PWM_SHLDR, MTR_DIR_SHLDR, false);

// static Sensor::CurrentSensor currentSensor(ELBW_CRNT_SNS_SPI_CLK, ELBW_CRNT_SNS_SPI_MISO, ELBW_CRNT_SNS_SPI_CS);
// TODO: Add once current sensor driver works

static PID::PID velPID({1, 0, 0, -1, 1, 0, false, false});
static PID::PID posPID({1, 0, 0, -1, 1, 0, false, false});
static PID::PID curPID({1, 0, 0, -1, 1, 0, false, false});

constexpr uint8_t PA04MAXCURRENT = 6;
constexpr float PA04MAXDEGPERSEC = std::numeric_limits<float>::infinity();  // TODO: figure out MAXDEGPERSEC of motors

static Controller::Velocity vel(motor, encoder, std::nullopt, velPID, PA04MAXDEGPERSEC, PA04MAXCURRENT, LIM_SHLDR_DN,
                                LIM_SHLDR_UP);
static Controller::Position pos(motor, encoder, std::nullopt, posPID, PA04MAXDEGPERSEC, PA04MAXCURRENT, LIM_SHLDR_DN,
                                LIM_SHLDR_UP);
static Controller::Current cur(motor, encoder, std::nullopt, curPID, PA04MAXDEGPERSEC, PA04MAXCURRENT, LIM_SHLDR_DN,
                               LIM_SHLDR_UP);
static Controller::OpenLoop open(motor, encoder, std::nullopt, PA04MAXDEGPERSEC, PA04MAXCURRENT, LIM_SHLDR_DN,
                                 LIM_SHLDR_UP);

static const Controller::ControlMap lut = {{HWBRIDGE::CONTROL::Mode::VELOCITY, &vel},
                                           {HWBRIDGE::CONTROL::Mode::POSITION, &pos},
                                           {HWBRIDGE::CONTROL::Mode::CURRENT, &cur},
                                           {HWBRIDGE::CONTROL::Mode::OPEN_LOOP, &open}};
}  // namespace Internal

static Controller::ActuatorControllerManager manager(Internal::lut, HWBRIDGE::CONTROL::Mode::OPEN_LOOP);

}  // namespace Shoulder
