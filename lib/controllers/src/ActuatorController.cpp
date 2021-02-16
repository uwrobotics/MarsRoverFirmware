#include "ActuatorController.h"

using namespace Controller;

void ActuatorController::setSetPoint(float sp) {
  m_setpoint.store(sp);
}

float ActuatorController::getSetPoint() const {
  return m_setpoint.load();
}

void ActuatorController::overrideCurrentChecks() {
  m_ignoreCurrentChecks.store(true);
}

void ActuatorController::reinstateCurrentChecks() {
  m_ignoreCurrentChecks.store(false);
}

void ActuatorController::overrideDegPerSecChecks() {
  m_ignoreDegPerSecChecks.store(true);
}

void ActuatorController::reinstateDegPerSecChecks() {
  m_ignoreDegPerSecChecks.store(false);
}
