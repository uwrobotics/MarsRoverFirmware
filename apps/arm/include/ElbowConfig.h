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

namespace Elbow {

namespace Internal {

static Encoder::AEAT6012 encoder({ELBW_ENC_SPI_CLK, ELBW_ENC_SPI_MISO, ELBW_CRNT_SNS_SPI_CS, 0});

static Actuator::DCMotor motor(MTR_PWM_ELBW, MTR_DIR_ELBW, false);

// static Sensor::CurrentSensor currentSensor(ELBW_CRNT_SNS_SPI_CLK, ELBW_CRNT_SNS_SPI_MISO, ELBW_CRNT_SNS_SPI_CS);
// TODO: Add once current sensor driver works

static PID::PID velPID({1, 0, 0, -1, 1, 0, false, false});
static PID::PID posPID({1, 0, 0, -1, 1, 0, false, false});
static PID::PID curPID({1, 0, 0, -1, 1, 0, false, false});

constexpr float ASSUNMAXCURRENT = 25.263;
constexpr float ASSUNMAXDEGPERSEC =
    std::numeric_limits<float>::infinity();  // TODO: figure out MAXDEGPERSEC of motors (35580?);

static Controller::Velocity vel(motor, encoder, std::nullopt, velPID, ASSUNMAXDEGPERSEC, ASSUNMAXCURRENT, LIM_ELBW_DN,
                                LIM_ELBW_UP);
static Controller::Position pos(motor, encoder, std::nullopt, posPID, ASSUNMAXDEGPERSEC, ASSUNMAXCURRENT, LIM_ELBW_DN,
                                LIM_ELBW_UP);
static Controller::Current cur(motor, encoder, std::nullopt, curPID, ASSUNMAXDEGPERSEC, ASSUNMAXCURRENT, LIM_ELBW_DN,
                               LIM_ELBW_UP);
static Controller::OpenLoop open(motor, encoder, std::nullopt, ASSUNMAXDEGPERSEC, ASSUNMAXCURRENT, LIM_ELBW_DN,
                                 LIM_ELBW_UP);

static const Controller::ControlMap lut = {{HWBRIDGE::CONTROL::Mode::Velocity, &vel},
                                           {HWBRIDGE::CONTROL::Mode::Position, &pos},
                                           {HWBRIDGE::CONTROL::Mode::Current, &cur},
                                           {HWBRIDGE::CONTROL::Mode::OpenLoop, &open}};
}  // namespace Internal

static Controller::ActuatorControllerManager manager(Internal::lut, HWBRIDGE::CONTROL::Mode::OpenLoop);

}  // namespace Elbow
