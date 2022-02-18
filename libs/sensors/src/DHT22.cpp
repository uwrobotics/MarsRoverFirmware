#include "DHT22.h"

using namespace Sensor;

DHT22::DHT22(PinName data) : m_data(data) {}

// Returns internal humidity value (and temperature? Since all transmitted at the same time...)
float DHT22::read() {
  // Not sure what the mutex is used for, maybe ask later?
  return m_humidity;
}

float DHT22::alternateRead() {
  return m_temperature;
}

// bool DHT22::getStatus() {}

// bool DHT22::reset() {}

bool DHT22::update() {
  // need to probably use the DigitalInOut type for getting the data from the sensor
  // Setup a 2 second loop of sorts? or 80 microseconds? Refer to Arduino code
  // Needs to check if 2 seconds have been passed already since last call
}