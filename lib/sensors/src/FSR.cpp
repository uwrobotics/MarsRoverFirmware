#include "FSR.h"

force_sensor::FSR::FSR(const PinName read_pin, const int sensitivity_resistor, const int VCC)
    : m_read_pin(read_pin), m_sensitivity_resistor(sensitivity_resistor), m_VCC(VCC) {}

force_sensor::FSR& force_sensor::FSR::operator=(const force_sensor::FSR& copy) {
  this->m_sensitivity_resistor = copy.m_sensitivity_resistor;
  this->m_VCC                  = copy.m_VCC;
  return *this;
}

int force_sensor::FSR::getForce(float& force) {
  float fsrConductance{calculateConductance()};
  // error chceking
  if (fsrConductance != -1) {
    force = (fsrConductance <= 1000) ? fsrConductance / BIG_NEWTON_CONVERSION_FACTOR
                                     : (fsrConductance - LARGE_FORCE) / SMALL_NEWTON_CONVERSION_FACTOR;
    return 0;

  } else
    return -1;
}

float force_sensor::FSR::calculateFSRResistance(void) {
  float voltage{mapPulseWidthToMilliVolts()};
  // error checking
  if (voltage != -1) {
    // need to wait a little bit before continuing
    ThisThread::sleep_for(3ms);

    float fsrResistance = m_VCC - voltage;

    fsrResistance *= m_sensitivity_resistor;

    fsrResistance /= voltage;

    return fsrResistance;
  } else
    return -1;
}
float force_sensor::FSR::calculateConductance(void) {
  float fsrConductance{ONE_MICRO_OHM};
  float resistance{calculateFSRResistance()};
  // error checking
  if (resistance != -1) {
    fsrConductance /= resistance;
    return fsrConductance;
  } else
    return -1;
}

float force_sensor::FSR::mapPulseWidthToMilliVolts(void) {
  float value = m_read_pin.read();
  if (value <= 1.0 && value >= 0.0)
    return m_read_pin.read() * (m_VCC / MAX_ANALOG_READ_VALUE);
  else
    return -1;
}
