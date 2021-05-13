#pragma once
#include <atomic>
#include <functional>
#include <optional>

#include "Actuator.h"
#include "ActuatorController.h"
#include "CurrentSensor.h"
#include "Encoder.h"
#include "PID.h"
#include "units.h"

using namespace units::current;
using namespace units::angular_velocity;
using namespace units::angle;


namespace Controller {
class ActuatorController {
 public:
  // TODO: Enable current sensor checks once Current Sensor Driver exists
  ActuatorController(Actuator::Actuator &actuator, Encoder::Encoder &encoder,
                     const std::optional<std::reference_wrapper<Sensor::CurrentSensor> const> &currentSensor,
                     degrees_per_sec_t maxDegPerSec, ampere_t maxCurrent, PinName lowerLimit, PinName upperLimit,
                     bool ignoreDegPerSecChecks = false, bool ignoreCurrentChecks = true,
                     bool ignoreLimitSwitchChecks = false);
  virtual ~ActuatorController() = default;

  /* final functions cannot be overriden by children */
  virtual void setSetPoint(float sp) final;
  virtual float getSetPoint() const final;

  virtual void activateCurrentChecks() final;
  virtual void deactivateCurrentChecks() final;

  virtual void activateDegPerSecChecks() final;
  virtual void deactivateDegPerSecChecks() final;

  virtual void activateLimitSwitchChecks() final;
  virtual void deactivateLimitSwitchChecks() final;

  virtual bool reportAngleDeg(degree_t &angle) final;
  virtual bool reportAngularVelocityDegPerSec(degrees_per_sec_t &speed) final;

  /* functions to be overriden by children */
  virtual bool update()                                            = 0;
  virtual void reset()                                             = 0;
  virtual std::optional<std::reference_wrapper<PID::PID>> getPID() = 0;
  virtual void stop()                                              = 0;

 protected:
  virtual bool shouldStop() final;

  std::atomic<float> m_setpoint = {0};

  std::atomic<bool> m_ignoreDegPerSecChecks;
  std::atomic<bool> m_ignoreCurrentChecks;
  std::atomic<bool> m_ignoreLimitSwitchChecks;

  Actuator::Actuator &m_actuator;
  Encoder::Encoder &m_encoder;
  const std::optional<std::reference_wrapper<Sensor::CurrentSensor> const> &m_currentSensor;

  const degrees_per_sec_t m_maxDegPerSec;
  const ampere_t m_maxCurrent;

  DigitalIn m_lowerLimit, m_upperLimit;
};
}  // namespace Controller
