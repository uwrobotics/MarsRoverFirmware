#pragma once

#include <optional>

#include "Actuator.h"
#include "ActuatorController.h"
#include "CurrentSensor.h"
#include "Encoder.h"

namespace Controller {
class OpenLoop final : public ActuatorController {
 public:
  OpenLoop(Actuator::Actuator *actuator, Encoder::Encoder *encoder, Sensor::CurrentSensor *currentSensor,
           float maxDegPerSec, float maxCurrent, std::optional<DigitalIn *> lowerLimit,
           std::optional<DigitalIn *> upperLimit);

  void stop() override;
  void reset() override;
  bool update() override;

  bool reportAngleDeg(float &angle) override;
  bool reportAngularVelocityDegPerSec(float &speed) override;

  std::optional<PID::PID *> getPID() override;

  bool shouldUpdate();

 private:
  Actuator::Actuator *m_actuator;
  Encoder::Encoder *m_encoder;
  Sensor::CurrentSensor *m_currentSensor;
  const float m_maxDegPerSec, m_maxCurrent;
  std::optional<DigitalIn *> m_lowerLimit, m_upperLimit;
};
}  // namespace Controller
