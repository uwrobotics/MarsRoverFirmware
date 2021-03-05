#pragma once

#include "ActuatorController.h"
namespace Controller {

class Position final : public ActuatorController {
 public:
  Position(Actuator::Actuator &actuator, Encoder::Encoder &encoder,
           const std::optional<std::reference_wrapper<Sensor::CurrentSensor> const> &currentSensor, PID::PID &pid,
           float maxDegPerSec, float maxCurrent, PinName lowerLimit, PinName upperLimit);
  void stop() override;
  bool update() override;
  void reset() override;
  std::optional<std::reference_wrapper<PID::PID>> getPID() override;

 private:
  PID::PID &m_pid;
};
}  // namespace Controller
