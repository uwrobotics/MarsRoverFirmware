#ifndef VOLTAGE_DIVIDER_THERMISTOR_H
#define VOLTAGE_DIVIDER_THERMISTOR_H
#include "mbed.h"

struct VoltageDividerThermistorConfig {
  PinName thermistor_vout_pin;
  float thermistor_b;
  float thermistor_vin;
  float expected_room_temp;
  float thermistor_room_temp_resistance;
  float voltage_divider_resistance;
};

// https://industrial.panasonic.com/cdbs/www-data/pdf/AUA0000/AUA0000C8.pdf thermistor datasheet
class VoltageDividerThermistor {
 private:
  float m_b;    // kelvins. Based on Resistances of 25 and 50 , part number ERT-J1VV104J
  float m_vin;  // might need to be in bytes
  // TODO need to confirm expected room temp.
  float m_room_temp;  // degrees in kelvin - 22 celcius
  float m_thermistor_room_temp_resistance;
  float m_voltage_divider_resistance;
  AnalogIn m_thermistor_voltage;

 public:
  VoltageDividerThermistor(VoltageDividerThermistorConfig thermistor_config);
  ~VoltageDividerThermistor();
  float readThermistorCelcius();
};

#endif /* VOLTAGE_DIVIDER_THERMISTOR_H */