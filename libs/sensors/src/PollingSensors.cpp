#include "PollingSensors.h"

#include "Logger.h"

using namespace Sensor;

PollingSensors::PollingSensors() : moisture_in_adc(moisture_in), CO2_in_adc(co2_in) {}

// The function currently logs a status message. Can be changed to return just a float indicating the sensor value
float moisture_monitoring() {
  float moisture_reading = (moisture_in_adc.read_voltage() * 5.0) / 1023.0;

  if (moisture_reading < 0 && moisture_reading > 900) {
    Utility::logger << "!!! MOISTURE FAULT:" << moisture_reading << "\n";
    return -1;
  }
  return moisture_reading;
}

float C02_monitoring() {
  float CO2_reading = CO2_in_adc.read_voltage() * 1000;

  if (CO2_reading <= 0) {
    Utility::logger << "!!! CO2 FAULT:" << CO2_reading << "\n";
    return -1;
  } else if (CO2_reading < 400) {
    Utility::logger << "!!! CO2 PREHEATING:" << CO2_reading << "\n";
    return 0;
  }
  float concentration = ((CO2_reading - 400) * 50.0) / 16.0;
  return concentration;
}
