#pragma once

#include <numeric>

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

namespace Wrist {

namespace Internal {

static Encoder::Pololu37D leftEncoder({ENC_QUAD_WRST_LHS_A, ENC_QUAD_WRST_LHS_B, NC, 0,
                                       GPIO::QEI::Encoding::X2_ENCODING});
static Encoder::Pololu37D rightEncoder({ENC_QUAD_WRST_RHS_A, ENC_QUAD_WRST_RHS_B, NC, 0,
                                        GPIO::QEI::Encoding::X2_ENCODING});

static Actuator::DCMotor leftMotor(MTR_PWM_WRST_LHS, MTR_DIR_WRST_LHS, false);
static Actuator::DCMotor rightMotor(MTR_PWM_WRST_RHS, MTR_DIR_WRST_RHS, false);

/* TODO: Can we use wrist limit switches if motors are individually addressable */

static Sensor::CurrentSensor leftCurrentSensor(WRSTL_CRNT_SNS_SPI_CLK, WRSTL_CRNT_SNS_SPI_MISO, WRSTL_CRNT_SNS_SPI_CS),
    rightCurrentSensor(WRSTR_CRNT_SNS_SPI_CLK, WRSTR_CRNT_SNS_SPI_MISO, WRSTR_CRNT_SNS_SPI_CS);

static PID::PID leftVelPID({1, 0, 0, -1, 1, 0, false, false});
static PID::PID leftPosPID({1, 0, 0, -1, 1, 0, false, false});
static PID::PID leftCurPID({1, 0, 0, -1, 1, 0, false, false});

static PID::PID rightVelPID({1, 0, 0, -1, 1, 0, false, false});
static PID::PID rightPosPID({1, 0, 0, -1, 1, 0, false, false});
static PID::PID rightCurPID({1, 0, 0, -1, 1, 0, false, false});

constexpr uint8_t pololuMaxCurrent = 3;
constexpr float pololumaxDegPerSec =
    std::numeric_limits<float>::infinity();  // TODO: figure out maxDegPerSec of motors (1680?)

static Controller::Velocity leftVel(&leftMotor, &leftEncoder, &leftCurrentSensor, &leftVelPID, pololumaxDegPerSec,
                                    pololuMaxCurrent, NC, NC);
static Controller::Position leftPos(&leftMotor, &leftEncoder, &leftCurrentSensor, &leftPosPID, pololumaxDegPerSec,
                                    pololuMaxCurrent, NC, NC);
static Controller::Current leftCur(&leftMotor, &leftEncoder, &leftCurrentSensor, &leftCurPID, pololumaxDegPerSec,
                                   pololuMaxCurrent, NC, NC);
static Controller::OpenLoop leftOpen(&leftMotor, &leftEncoder, &leftCurrentSensor, pololumaxDegPerSec, pololuMaxCurrent,
                                     NC, NC);

static Controller::Velocity rightVel(&rightMotor, &rightEncoder, &rightCurrentSensor, &rightVelPID, pololumaxDegPerSec,
                                     pololuMaxCurrent, NC, NC);
static Controller::Position rightPos(&rightMotor, &rightEncoder, &rightCurrentSensor, &rightPosPID, pololumaxDegPerSec,
                                     pololuMaxCurrent, NC, NC);
static Controller::Current rightCur(&rightMotor, &rightEncoder, &rightCurrentSensor, &rightCurPID, pololumaxDegPerSec,
                                    pololuMaxCurrent, NC, NC);
static Controller::OpenLoop rightOpen(&rightMotor, &rightEncoder, &rightCurrentSensor, pololumaxDegPerSec,
                                      pololuMaxCurrent, NC, NC);

static const LookupTable::LookupTable<HWBRIDGE::CONTROL::Mode, Controller::ActuatorController *> leftLut = {
    {HWBRIDGE::CONTROL::Mode::Velocity, &leftVel},
    {HWBRIDGE::CONTROL::Mode::Position, &leftPos},
    {HWBRIDGE::CONTROL::Mode::Current, &leftCur},
    {HWBRIDGE::CONTROL::Mode::OpenLoop, &leftOpen}};
static const LookupTable::LookupTable<HWBRIDGE::CONTROL::Mode, Controller::ActuatorController *> rightLut = {
    {HWBRIDGE::CONTROL::Mode::Velocity, &rightVel},
    {HWBRIDGE::CONTROL::Mode::Position, &rightPos},
    {HWBRIDGE::CONTROL::Mode::Current, &rightCur},
    {HWBRIDGE::CONTROL::Mode::OpenLoop, &rightOpen}};
}  // namespace Internal

static Controller::ActuatorControllerManager leftManager(Internal::leftLut, HWBRIDGE::CONTROL::Mode::OpenLoop);
static Controller::ActuatorControllerManager rightManager(Internal::rightLut, HWBRIDGE::CONTROL::Mode::OpenLoop);

}  // namespace Wrist
