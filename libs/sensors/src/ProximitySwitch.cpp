#include "ProximitySwitch.h"

#include "Logger.h"

using namespace Sensor;

ProximitySwitch::ProximitySwitch(PinName proximity_in) : m_proximity_in_adc(proximity_in) {}

/*https://www.dfrobot.com/product-2025.html*/
float ProximitySwitch::read() {
  if (m_proximity_in_adc.is_connected()) {
    return m_proximity_in_adc.read()
  } else {
    Utility::logger << "ERROR: Sensor pin not connected";
    return 2;
  }
}
