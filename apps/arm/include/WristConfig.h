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

// static Sensor::CurrentSensor leftCurrentSensor(WRSTL_CRNT_SNS_SPI_CLK, WRSTL_CRNT_SNS_SPI_MISO,
// WRSTL_CRNT_SNS_SPI_CS),
//     rightCurrentSensor(WRSTR_CRNT_SNS_SPI_CLK, WRSTR_CRNT_SNS_SPI_MISO, WRSTR_CRNT_SNS_SPI_CS);
// TODO: Add once current sensor driver works

static PID::PID leftVelPID({1, 0, 0, -1, 1, 0, false, false});
static PID::PID leftPosPID({1, 0, 0, -1, 1, 0, false, false});
static PID::PID leftCurPID({1, 0, 0, -1, 1, 0, false, false});

static PID::PID rightVelPID({1, 0, 0, -1, 1, 0, false, false});
static PID::PID rightPosPID({1, 0, 0, -1, 1, 0, false, false});
static PID::PID rightCurPID({1, 0, 0, -1, 1, 0, false, false});

constexpr uint8_t POLOLUMAXCURRENT = 3;
constexpr float POLOLUMAXDEGPERSEC =
    std::numeric_limits<float>::infinity();  // TODO: figure out MAXDEGPERSEC of motors (1680?)

static Controller::Velocity leftVel(leftMotor, leftEncoder, std::nullopt, leftVelPID, POLOLUMAXDEGPERSEC,
                                    POLOLUMAXCURRENT, NC, NC);
static Controller::Position leftPos(leftMotor, leftEncoder, std::nullopt, leftPosPID, POLOLUMAXDEGPERSEC,
                                    POLOLUMAXCURRENT, NC, NC);
static Controller::Current leftCur(leftMotor, leftEncoder, std::nullopt, leftCurPID, POLOLUMAXDEGPERSEC,
                                   POLOLUMAXCURRENT, NC, NC);
static Controller::OpenLoop leftOpen(leftMotor, leftEncoder, std::nullopt, POLOLUMAXDEGPERSEC, POLOLUMAXCURRENT, NC,
                                     NC);

static Controller::Velocity rightVel(rightMotor, rightEncoder, std::nullopt, rightVelPID, POLOLUMAXDEGPERSEC,
                                     POLOLUMAXCURRENT, NC, NC);
static Controller::Position rightPos(rightMotor, rightEncoder, std::nullopt, rightPosPID, POLOLUMAXDEGPERSEC,
                                     POLOLUMAXCURRENT, NC, NC);
static Controller::Current rightCur(rightMotor, rightEncoder, std::nullopt, rightCurPID, POLOLUMAXDEGPERSEC,
                                    POLOLUMAXCURRENT, NC, NC);
static Controller::OpenLoop rightOpen(rightMotor, rightEncoder, std::nullopt, POLOLUMAXDEGPERSEC, POLOLUMAXCURRENT, NC,
                                      NC);

static const Controller::ControlMap leftLut  = {{HWBRIDGE::CONTROL::Mode::VELOCITY, &leftVel},
                                               {HWBRIDGE::CONTROL::Mode::POSITION, &leftPos},
                                               {HWBRIDGE::CONTROL::Mode::CURRENT, &leftCur},
                                               {HWBRIDGE::CONTROL::Mode::OPEN_LOOP, &leftOpen}};
static const Controller::ControlMap rightLut = {{HWBRIDGE::CONTROL::Mode::VELOCITY, &rightVel},
                                                {HWBRIDGE::CONTROL::Mode::POSITION, &rightPos},
                                                {HWBRIDGE::CONTROL::Mode::CURRENT, &rightCur},
                                                {HWBRIDGE::CONTROL::Mode::OPEN_LOOP, &rightOpen}};
}  // namespace Internal

static Controller::ActuatorControllerManager leftManager(Internal::leftLut, HWBRIDGE::CONTROL::Mode::OPEN_LOOP);
static Controller::ActuatorControllerManager rightManager(Internal::rightLut, HWBRIDGE::CONTROL::Mode::OPEN_LOOP);

}  // namespace Wrist
