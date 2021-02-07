#pragma once
#include <atomic>

#include "PID.h"

namespace Controller {
class ActuatorController {
 public:
  virtual ~ActuatorController() = default;

  /* final functions cannot be overriden by children */
  virtual void setSetPoint(float sp) final;
  virtual float getSetPoint() const final;

  virtual void overrideCurrentChecks() final;
  virtual void reinstateCurrentChecks() final;

  virtual void overrideDegPerSecChecks() final;
  virtual void reinstateDegPerSecChecks() final;

  /* functions to be overriden by children */
  virtual bool update() = 0;
  virtual void stop()   = 0;
  virtual void reset()  = 0;
  virtual bool reportAngleDeg(float &angle) = 0;
  virtual bool reportAngularVelocityDegPerSec(float &speed) = 0;

  virtual std::optional<PID::PID *> getPID() = 0;

 protected:
  std::atomic<float> m_sp                 = {0};
  std::atomic<bool> m_ignoreCurrentChecks = {false};
  std::atomic<bool> m_ignoreRPMChecks     = {false};
};
}  // namespace Controller
