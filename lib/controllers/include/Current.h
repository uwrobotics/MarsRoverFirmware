#pragma once

#include "ActuatorController.h"
namespace Controller {

class Current final : public ActuatorController {
 public:
  Current(Actuator::Actuator &actuator, Encoder::Encoder &encoder,
          const std::optional<std::reference_wrapper<Sensor::CurrentSensor> const> &currentSensor, PID::PID &pid,
          float maxDegPerSec, float maxCurrent, PinName lowerLimit, PinName upperLimit,
          bool ignoreDegPerSecChecks = false, bool ignoreCurrentChecks = true, bool ignoreLimitSwitchChecks = false);
  bool update() override;
  void stop() override;
  void reset() override;
  std::optional<std::reference_wrapper<PID::PID>> getPID() override;

 private:
  PID::PID &m_pid;
};
}  // namespace Controller
