#pragma once

#include "ActuatorController.h"

namespace Controller {

class Velocity final : public ActuatorController {
 public:
  Velocity(Actuator::Actuator &actuator, Encoder::Encoder &encoder,
           const std::optional<std::reference_wrapper<Sensor::CurrentSensor> const> &currentSensor, PID::PID &pid,
           float maxDegPerSec, float maxCurrent, PinName lowerLimit, PinName upperLimit,
           bool ignoreDegPerSecChecks = false, bool ignoreCurrentChecks = true, bool ignoreLimitSwitchChecks = false);
  void stop() override;
  bool update() override;
  bool reset() override;
  std::optional<std::reference_wrapper<PID::PID>> getPID() override;

 private:
  PID::PID &m_pid;
};
}  // namespace Controller
