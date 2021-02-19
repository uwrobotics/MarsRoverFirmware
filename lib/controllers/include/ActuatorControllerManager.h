#pragma once

#include <optional>

#include "ActuatorController.h"
#include "LookupTable.h"
#include "hw_bridge.h"

namespace Controller {
using ControlMap = Utility::LookupTable<HWBRIDGE::CONTROL::Mode, ActuatorController *>;
class ActuatorControllerManager {
 public:
  ActuatorControllerManager(const ControlMap &lut, HWBRIDGE::CONTROL::Mode active);
  ActuatorControllerManager()                             = delete;
  ActuatorControllerManager(ActuatorControllerManager &)  = delete;
  ActuatorControllerManager(ActuatorControllerManager &&) = delete;

  bool switchControlMode(HWBRIDGE::CONTROL::Mode newControlType);
  HWBRIDGE::CONTROL::Mode getActiveControlMode() const;
  ActuatorController *getActiveController() const;

 private:
  const Utility::LookupTable<HWBRIDGE::CONTROL::Mode, ActuatorController *> m_lut;

  std::atomic<HWBRIDGE::CONTROL::Mode> m_mode;
  std::atomic<ActuatorController *> m_controller;
};
}  // namespace Controller