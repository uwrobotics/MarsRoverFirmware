#include "Voltage_Divider_Thermistor.h"

#include "math.h"

constexpr float KELVIN_TO_CELCIUS_CONVERSION = 273.15;

VoltageDividerThermistor::VoltageDividerThermistor(VoltageDividerThermistorConfig thermistor_config)
    : m_thermistor_voltage(thermistor_config.thermistor_vout_pin) {
  m_b   = thermistor_config.thermistor_b;    // kelvins. Based on Resistances of 25 and 50 , part number ERT-J1VV104J
  m_vin = thermistor_config.thermistor_vin;  // might need to be in bytes
  m_room_temp                       = thermistor_config.expected_room_temp;  // degrees in kelvin - 22 celcius
  m_thermistor_room_temp_resistance = thermistor_config.thermistor_room_temp_resistance;  // ohms
  m_voltage_divider_resistance      = thermistor_config.voltage_divider_resistance;       // ohms
}

VoltageDividerThermistor::~VoltageDividerThermistor() {}

float VoltageDividerThermistor::readThermistorCelcius() {
  // voltage divider to find resistance of thermister
  float thermister_resistance = m_voltage_divider_resistance * ((m_vin / m_thermistor_voltage.read()) - 1);

  /*  convert resistance to temp (steinhart and hart eqn)
      A,B,C are Stienhart-hart coefficients which depend on the thermistor and temperature range of interest.
      (https://industrial.panasonic.com/cdbs/www-data/pdf/AUA0000/AUA0000C8.pdf)
      https://en.wikipedia.org/wiki/Steinhart%E2%80%93Hart_equation
      Need to test the thermistor for resistance at 3 different temperatures and use results to compute A,B,C

      Alernatively use B value from datasheet which is less accurate but does not require experimental data
      https://www.allaboutcircuits.com/projects/measuring-temperature-with-an-ntc-thermistor/
  */
  float measured_thermistor_temp_k =
      (m_room_temp * m_b) / (m_b + (m_room_temp * log((thermister_resistance / m_thermistor_room_temp_resistance))));

  return measured_thermistor_temp_k - KELVIN_TO_CELCIUS_CONVERSION;  // convert Kelvins to celcius
}
