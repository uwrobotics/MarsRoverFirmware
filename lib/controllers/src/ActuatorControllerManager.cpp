#include "ActuatorControllerManager.h"

using namespace Controller;

ActuatorControllerManager::ActuatorControllerManager(
    const LookupTable::LookupTable<HWBRIDGE::CONTROL::Mode, ActuatorController *> &lut, HWBRIDGE::CONTROL::Mode active)
    : m_lut(lut), m_mode(active), m_controller(nullptr) {
  MBED_ASSERT(m_lut.find(active) != m_lut.end());
  m_controller = m_lut.at(m_mode).value();
}

bool ActuatorControllerManager::switchControlMode(HWBRIDGE::CONTROL::Mode newControlType) {
  m_lut.at(getActiveControlMode()).value()->stop();
  if (auto temp = m_lut.at(newControlType); temp != std::nullopt) {
    m_mode.store(newControlType);
    m_controller.store(m_lut.at(m_mode).value());
    m_controller.load()->reset();
    return true;
  }
  return false;
}

HWBRIDGE::CONTROL::Mode ActuatorControllerManager::getActiveControlMode() const {
  return m_mode.load();
}

ActuatorController *ActuatorControllerManager::getActiveController() const {
  return m_controller.load();
}