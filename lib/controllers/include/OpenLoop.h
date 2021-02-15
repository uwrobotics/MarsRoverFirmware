#pragma once

#include <functional>
#include <optional>

#include "Actuator.h"
#include "ActuatorController.h"
#include "CurrentSensor.h"
#include "Encoder.h"

namespace Controller {
class OpenLoop final : public ActuatorController {
 public:
  OpenLoop(Actuator::Actuator &actuator, Encoder::Encoder &encoder,
           const std::optional<std::reference_wrapper<Sensor::CurrentSensor> const> &currentSensor, float maxDegPerSec,
           float maxCurrent, PinName lowerLimit, PinName upperLimit);

  void stop() override;
  void reset() override;
  bool update() override;

  bool reportAngleDeg(float &angle) override;
  bool reportAngularVelocityDegPerSec(float &speed) override;

  std::optional<std::reference_wrapper<PID::PID>> getPID() override;

  bool shouldUpdate();

 private:
  Actuator::Actuator &m_actuator;
  Encoder::Encoder &m_encoder;
  const std::optional<std::reference_wrapper<Sensor::CurrentSensor> const> &m_currentSensor;
  const float m_maxDegPerSec, m_maxCurrent;
  DigitalIn m_lowerLimit, m_upperLimit;
};
}  // namespace Controller
