#pragma once

#include "mbed.h"

namespace force_sensor {

class FSR {
 public:
  // rule-of-5
  FSR() = delete;
  explicit FSR(const PinName read_pin, const int sensitivity_resistor = DEFAULT_RESISTOR,
               const int VCC = MAX_VOLTAGE_MILLI_VOLTS);
  FSR(const FSR& copy)            = delete;
  explicit FSR(const FSR&& mover) = delete;
  FSR& operator                   =(const FSR& copy);
  FSR& operator=(FSR&& mover) = delete;
  ~FSR()                      = default;

  // @param - force:
  //          gets filled with the force measurements in Newtons - pass by reference
  // @return - 0 for successful read and -1 on failed read
  int getForce(float& force);

 private:
  // special constants needed for calculation
  constexpr static int LARGE_FORCE{1000};  // ooh magic number
  constexpr static int ONE_MICRO_OHM{1000000};
  constexpr static int DEFAULT_RESISTOR{10000};             // a good sensitivity level for determining force
  constexpr static int BIG_NEWTON_CONVERSION_FACTOR{80};    // ooh magic number pt2
  constexpr static int SMALL_NEWTON_CONVERSION_FACTOR{30};  // ooh magic number pt3
  constexpr static int MAX_VOLTAGE_MILLI_VOLTS{5000};
  constexpr static int MAX_ANALOG_READ_VALUE{1};  //  mbed AnalogIn read member function reads a value from 0.0 - 1.0

  // Pin to read analog values
  AnalogIn m_read_pin;

  // This resistor will determine how sensitive the force readings are
  // The higher the resistance the less sensitive it is to reading
  int m_sensitivity_resistor;

  // voltage power source i.e. either 3000 millivolts or 5000 millivolts - use millivolts
  int m_VCC;

  // Helper function to calculate conductance from resistance provided from Force Sensitive Resistor
  // Conductance is used instead of resistance because force vs resistance is logarithmic while
  // Force vs Conductance is almost Linear
  float calculateConductance(void);

  // Helper function to map analog reading 0.0 - 1.0 to voltage 0.0 - 5.0 volts (or different voltage value)
  float mapPulseWidthToMilliVolts(void);

  // Helper function to Calculate Force Sensor Resistance based on the formula for FSR400s Series Manual
  //
  //   Voltage Divider:
  //
  //   Vout = (default resistor)*(voltage measured from analog reading) / (default resistor + FSR)
  //
  //   FSR  = ((Vcc - (voltage measured from analog reading)) * (default resistor)) / (voltage measured from analog
  //   reading)
  //
  float calculateFSRResistance(void);
};

}  // namespace force_sensor
