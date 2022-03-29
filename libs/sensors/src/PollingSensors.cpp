#include "PollingSensors.h"

#include "Logger.h"

using namespace Sensor;

PollingSensors::PollingSensors(PinName moisture_in, PinName co2_in)
    : m_moisture_in_adc(moisture_in), m_CO2_in_adc(co2_in) {}

/*https://www.dfrobot.com/product-599.html*/
float PollingSensors::moisture_monitoring() {
  float moisture_reading = (m_moisture_in_adc.read_voltage() * 950) / 4.2;

  if (moisture_reading < 0 && moisture_reading > 950) {
    Utility::logger << "!!! MOISTURE FAULT:" << moisture_reading << "\n";
    return -1;
  }
  return moisture_reading;
}

/*https://www.dfrobot.com/product-1549.html*/
float PollingSensors::C02_monitoring() {
  float CO2_reading = m_CO2_in_adc.read_voltage() * 1000;

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
