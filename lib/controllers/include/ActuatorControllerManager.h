#pragma once

#include <optional>

#include "ActuatorController.h"
#include "LookupTable.h"
#include "hw_bridge.h"

namespace Controller {
class ActuatorControllerManager {
 public:
  ActuatorControllerManager(const LookupTable::LookupTable<HWBRIDGE::CONTROL::Mode, ActuatorController *> &lut,
                            HWBRIDGE::CONTROL::Mode active);
  ActuatorControllerManager()                             = delete;
  ActuatorControllerManager(ActuatorControllerManager &)  = delete;
  ActuatorControllerManager(ActuatorControllerManager &&) = delete;

  bool switchControlMode(HWBRIDGE::CONTROL::Mode newControlType);
  HWBRIDGE::CONTROL::Mode getActiveControlMode() const;
  ActuatorController *getActiveController() const;

 private:
  const LookupTable::LookupTable<HWBRIDGE::CONTROL::Mode, ActuatorController *> m_lut;

  std::atomic<HWBRIDGE::CONTROL::Mode> m_mode;
  std::atomic<ActuatorController *> m_controller;
};
}  // namespace Controller