#pragma once

#include <optional>

#include "Actuator.h"
#include "ActuatorController.h"
#include "CurrentSensor.h"
#include "Encoder.h"
#include "PID.h"

namespace Controller {

class BasicControl : public ActuatorController {
 public:
  BasicControl(Actuator::Actuator *actuator, Encoder::Encoder *encoder, Sensor::CurrentSensor *currentSensor,
               PID::PID *pid, float maxDegPerSec, float maxCurrent, std::optional<DigitalIn *> lowerLimit,
               std::optional<DigitalIn *> upperLimit);
  BasicControl(BasicControl &)  = delete;
  BasicControl(BasicControl &&) = delete;
  ~BasicControl()               = default;

  void stop() override final;
  void reset() override final;

  bool reportAngleDeg(float &angle) override;
  bool reportAngularVelocityDegPerSec(float &speed) override;

  std::optional<PID::PID *> getPID() override;

 protected:
  Actuator::Actuator *m_actuator;
  Encoder::Encoder *m_encoder;
  Sensor::CurrentSensor *m_currentSensor;

  PID::PID *m_pid;

  const float m_maxDegPerSec, m_maxCurrent;

  std::optional<DigitalIn *> m_upperLimit, m_lowerLimit;
};
}  // namespace Controller
