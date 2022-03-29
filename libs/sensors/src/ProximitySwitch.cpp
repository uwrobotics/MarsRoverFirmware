#include "ProximitySwitch.h"

using namespace Sensor;

ProximitySwitch::ProximitySwitch(PinName proximity_in) : m_proximity_in_adc(proximity_in) {}

/*https://www.dfrobot.com/product-2025.html*/
float ProximitySwitch::read() {
  float proximity_reading = m_proximity_in_adc.read();

  return proximity_reading;
}
