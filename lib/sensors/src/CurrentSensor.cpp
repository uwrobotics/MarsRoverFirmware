#include "CurrentSensor.h"

using namespace Sensor;

CurrentSensor::CurrentSensor(PinName clk, PinName miso, PinName cs) {
  MBED_WARNING(MBED_MAKE_ERROR(MBED_MODULE_APPLICATION, MBED_ERROR_CODE_UNSUPPORTED),
               "CURRENT SENSOR CLASS IS UNIMPLEMENTED");  // todo
}

float CurrentSensor::read() {
  MBED_WARNING(MBED_MAKE_ERROR(MBED_MODULE_APPLICATION, MBED_ERROR_CODE_UNSUPPORTED),
               "CURRENT SENSOR CLASS IS UNIMPLEMENTED");  // todo
  return -1;
}

float CurrentSensor::alternateRead() {
  MBED_WARNING(MBED_MAKE_ERROR(MBED_MODULE_APPLICATION, MBED_ERROR_CODE_UNSUPPORTED),
               "CURRENT SENSOR CLASS IS UNIMPLEMENTED");  // todo
  return -1;
}

bool CurrentSensor::getStatus() const {
  MBED_WARNING(MBED_MAKE_ERROR(MBED_MODULE_APPLICATION, MBED_ERROR_CODE_UNSUPPORTED),
               "CURRENT SENSOR CLASS IS UNIMPLEMENTED");  // todo
  return false;
}

bool CurrentSensor::reset() {
  MBED_WARNING(MBED_MAKE_ERROR(MBED_MODULE_APPLICATION, MBED_ERROR_CODE_UNSUPPORTED),
               "CURRENT SENSOR CLASS IS UNIMPLEMENTED");  // todo
  return false;
}

bool CurrentSensor::update() {
  MBED_WARNING(MBED_MAKE_ERROR(MBED_MODULE_APPLICATION, MBED_ERROR_CODE_UNSUPPORTED),
               "CURRENT SENSOR CLASS IS UNIMPLEMENTED");  // todo
  return false;
}