#include "CANMsg.h"
#include "INA_226.h"
#include "hw_bridge.h"
#include "mbed.h"

// if in range returns 1, otherwise returns 0
bool inToleranceRange(float tolerance, float expected_val, float measured_val) {
  return ((expected_val + tolerance < measured_val) || (expected_val - tolerance > measured_val)) ? 0 : 1;
}

///////////////////////////////////////////////////////////////////////////
//  This is a testing app for the INA-226 Current Sensing chip           //
///////////////////////////////////////////////////////////////////////////
//  This app is designed to be flashed onto the MCU then run from top to bottom
//  Specific values such as the expected current and voltage must first be
//  configured to ensure proper testing of the functionality.
//  The remaining functionality to configure the modes of the chip
//  have been preprogrammed and already contain their own testing parameters
//  A serial port should also be configured to listen into the output
//  to determine which tests have passed/failed.
///////////////////////////////////////////////////////////////////////////

int main() {
  // Modify the sensor address, the shunt resistance and the maximum expected current
  // these values follow the pin names in the respective order
  char* test_name = (char*)"Test";
  u_int8_t addr   = 128;
  // i2c pins may be incorrect
  // name of sensor, SDA pin, SCL pin, addr of chip, shunt resistance, max exptect current
  ComponentConfig testing_config = {test_name, I2C_SDA, I2C_SCL, addr, 0.003, 28.75};

  printf("Attempting to create sensor object");
  INA_226 testing_sensor(testing_config);
  printf("Created INA sensor object");

  printf("Calibrating Sensor\r\n");
  testing_sensor.calibrateSensor();
  printf("Sensor Calibrated\r\n");

  /////////////////////////
  // TEST CURRENT SAMPLING//
  /////////////////////////
  // Modify the EXPECTED_CURRENT to be the test current sent to the sensor
  float current_tolerance = 0.01;
  float expected_current  = 5;
  float measured_current  = testing_sensor.getCurrentData();
  if (inToleranceRange(current_tolerance, expected_current, measured_current)) {
    printf("Measured current is within expected range. Measured value: %f\r\n", measured_current);
  } else {
    printf("Measured current is not within expected range. Measured value: %f\r\n", measured_current);
  }

  /////////////////////////
  // TEST VOLTAGE SAMPLING//
  /////////////////////////
  // Modify the EXPECTED_VOLTAGE to be the test voltage sent to the sensor
  float voltage_tolerance = 0.01;
  float expected_voltage  = 5;
  float measured_voltage  = testing_sensor.getVoltageData();
  if (inToleranceRange(voltage_tolerance, expected_voltage, measured_voltage)) {
    printf("Measured voltage is within expected range. Measured value: %f\r\n", measured_voltage);
  } else {
    printf("Measured voltage is within expected range. Measured value: %f\r\n", measured_voltage);
  }

  /////////////////////////
  // TEST  POWER  SAMPLING//
  /////////////////////////
  // Modify the EXPECTED_POWER to be the test power sent to the sensor
  float power_tolerance = 0.01;
  float expected_power  = 5;
  float measured_power  = testing_sensor.getPowerData();
  if (inToleranceRange(power_tolerance, expected_power, measured_power)) {
    printf("Measured power is within expected range. Measured value: %f\r\n", measured_power);
  } else {
    printf("Measured power is within expected range. Measured value: %f\r\n", measured_power);
  }

  // These tests run on their own without intervention

  /////////////////////////////////
  // TEST SETTING CONFIG REGISTER //
  /////////////////////////////////
  // test config > 0 100 111 011 011 001
  SensorModes test_config      = {0x00, 0x04, 0x07, 0x03, 0x03};
  u_int16_t test_config_binary = 0x4ED9;  // 0100111011011001
  testing_sensor.configureSensor(test_config);
  auto config_register = testing_sensor.readConfigRegister();
  if (config_register != test_config_binary) {
    printf("Incorrect config register setting recieved: %u\r\n", config_register);
  }

  ///////////////////////////
  // TEST SETTING  DEFAULTS //
  ///////////////////////////
  SensorModes reset_sensor_config = {0x01, 0x00, 0x00, 0x00, 0x00};
  u_int16_t default_settings      = 0x127;  // 0 000 100 100 111
  testing_sensor.configureSensor(reset_sensor_config);
  config_register = testing_sensor.readConfigRegister();
  if (config_register != default_settings) {
    printf("Incorrect default config register setting recieved %u\r\n", config_register);
  }

  ////////////////////////////////
  // TEST SETTING ALERT REGISTER //
  ////////////////////////////////
  u_int16_t alert_limit = 0x01;
  testing_sensor.setAlertLimit(alert_limit);
  u_int16_t alert = testing_sensor.getAlertLimit();
  if (alert != alert_limit) {
    printf("Incorrect alert limit %u\n", alert);
  }

  ///////////////////////////////
  // TEST SETTING MASK REGISTER //
  ///////////////////////////////
  u_int16_t INA_control = 0xFFFF;  // 0x1111111111111111
  testing_sensor.setMaskEnableRegister(INA_control);
  u_int16_t mask_bits = testing_sensor.readMaskRegister();
  if (mask_bits != INA_control) {
    printf("Incorrect mask data %u\r\n", mask_bits);
  }
  printf("END OF TESTING\r\n");

  return 0;
}
