#include "ActuatorController.h"

using namespace Controller;

void ActuatorController::setSetPoint(float sp) {
  m_sp.store(sp);
}

float ActuatorController::getSetPoint() const {
  return m_sp.load();
}

void ActuatorController::overrideCurrentChecks() {
  m_ignoreCurrentChecks.store(true);
}

void ActuatorController::reinstateCurrentChecks() {
  m_ignoreCurrentChecks.store(false);
}

void ActuatorController::overrideDegPerSecChecks() {
  m_ignoreRPMChecks.store(true);
}

void ActuatorController::reinstateDegPerSecChecks() {
  m_ignoreRPMChecks.store(false);
}
