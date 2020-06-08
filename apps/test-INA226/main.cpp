#include "CANMsg.h"
#include "INA_226.h"
#include "mbed.h"
#include "rover_config.h"

// Serial communications for debugging
Serial pc(SERIAL_TX, SERIAL_RX, ROVER_DEFAULT_SERIAL_BAUD_RATE);

// if in range returns 1, otherwise returns 0
bool inToleranceRange(float tolerance, float expected_val, float measured_val) {
  return ((expected_val + tolerance < measured_val) || (expected_val - tolerance > measured_val)) ? 0 : 1;
}

int main() {
  // configure values for the testing sensor, TODO, need updated values
  ComponentConfig testing_config = {"TEST", CUR_SEN_I2C_SDA, CUR_SEN_I2C_SCL, 0x00, 0.003, 28.75};

  INA_226 testing_sensor(testing_config);

  pc.printf("Calibrating Sensor\r\n");
  testing_sensor.calibrateSensor();
  pc.printf("Sensor Calibrated\r\n");

  float current_tolerance = 0.01;
  float expected_current  = 5;
  float measured_current  = testing_sensor.getCurrentData();
  if (inToleranceRange(current_tolerance, expected_current, measured_current)) {
    pc.printf("Measured current is within expected range. Measured value: %f\r\n", measured_current);
  }

  float voltage_tolerance = 0.01;
  float expected_voltage  = 5;
  float measured_voltage  = testing_sensor.getVoltageData();
  if (inToleranceRange(voltage_tolerance, expected_voltage, measured_voltage)) {
    pc.printf("Measured voltage is within expected range. Measured value: %f\r\n", measured_voltage);
  }

  float power_tolerance = 0.01;
  float expected_power  = 5;
  float measured_power  = testing_sensor.getPowerData();
  if (inToleranceRange(power_tolerance, expected_power, measured_power)) {
    pc.printf("Measured power is within expected range. Measured value: %f\r\n", measured_power);
  }

  // test config > 0 100 111 011 011 001
  SensorModes test_config      = {0x00, 0x04, 0x07, 0x03, 0x03};
  u_int16_t test_config_binary = 0100111011011001;
  testing_sensor.configureSensor(test_config);
  if (!(testing_sensor.readConfigRegister() == test_config_binary)) {
    pc.printf("Incorrect config register setting recieved\r\n");
  }

  SensorModes reset_sensor_config = {0x01, 0x00, 0x00, 0x00, 0x00};
  u_int16_t default_settings      = 0x127;  // 0 000 100 100 111
  testing_sensor.configureSensor(reset_sensor_config);
  if (!(testing_sensor.readConfigRegister() == default_settings)) {
    pc.printf("Incorrect default config register setting recieved\r\n");
  }
}
