#include "CurrentSensor.h"

using namespace Sensor;

CurrentSensor::CurrentSensor(PinName clk, PinName miso, PinName cs) {
  MBED_ASSERT(false);
}

    bool CurrentSensor::read(float &sensorReading) {
  MBED_ASSERT(false);  // todo
  return false;
}

bool CurrentSensor::alternateRead(float &sensorReading) {
  MBED_ASSERT(false);  // todo
  return false;
}

bool CurrentSensor::getStatus() const {
  MBED_ASSERT(false);  // todo
  return false;
}

bool CurrentSensor::reset() {
  MBED_ASSERT(false);  // todo
  return false;
}
