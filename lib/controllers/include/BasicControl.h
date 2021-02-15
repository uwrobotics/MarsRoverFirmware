#pragma once

#include <functional>
#include <optional>

#include "Actuator.h"
#include "ActuatorController.h"
#include "CurrentSensor.h"
#include "Encoder.h"
#include "PID.h"
namespace Controller {

class BasicControl : public ActuatorController {
 public:
  BasicControl(Actuator::Actuator &actuator, Encoder::Encoder &encoder,
               const std::optional<std::reference_wrapper<Sensor::CurrentSensor> const> &currentSensor, PID::PID &pid,
               float maxDegPerSec, float maxCurrent, PinName lowerLimit, PinName upperLimit);
  BasicControl(BasicControl &)  = delete;
  BasicControl(BasicControl &&) = delete;
  ~BasicControl()               = default;

  void stop() override final;
  void reset() override final;

  bool reportAngleDeg(float &angle) override;
  bool reportAngularVelocityDegPerSec(float &speed) override;

  std::optional<std::reference_wrapper<PID::PID>> getPID() override;

 protected:
  Actuator::Actuator &m_actuator;
  Encoder::Encoder &m_encoder;
  const std::optional<std::reference_wrapper<Sensor::CurrentSensor> const> &m_currentSensor;

  PID::PID &m_pid;

  const float m_maxDegPerSec, m_maxCurrent;

  DigitalIn m_lowerLimit, m_upperLimit;

  virtual bool shouldUpdate() final;
};
}  // namespace Controller
