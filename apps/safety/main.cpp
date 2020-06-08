#include "CANMsg.h"
#include "INA_226.h"
#include "Voltage_Divider_Thermistor.h"
#include "can_config.h"
#include "mbed.h"
#include "rover_config.h"

/*
*************************** MAY BE DEPRECATED AS SENSORS MAY BE ON EVERY BOARD INSTEAD
***********************************

TODO
2. Add hardware signals(leds?) for testing
3. Functions to handle CAN
4. Functions to send back CAN messages

*************************** MAY BE DEPRECATED AS SENSORS MAY BE ON EVERY BOARD INSTEAD
***********************************
*/

// Serial communications for debugging
Serial pc(SERIAL_TX, SERIAL_RX, ROVER_DEFAULT_SERIAL_BAUD_RATE);

// CAN initialization
CAN can(CAN1_RX, CAN1_TX, ROVER_CANBUS_FREQUENCY);  // TODO: dont know the actual CAN id's, find out can id for safety
CANMsg rxMsg;
CANMsg txMsg;

// measurement thresholds, needs updating, comparison in celcius
constexpr float CURRENT_UPPER_LIM = 1;
constexpr float CURRENT_LOWER_LIM = 0;
constexpr float VOLTAGE_UPPER_LIM = 1;
constexpr float VOLTAGE_LOWER_LIM = 0;
constexpr float TEMP_UPPER_LIM    = 1;
constexpr float TEMP_LOWER_LIM    = 0;

constexpr int NUM_SAFETY_SENSORS = 6;

enum SENSORNAMES { elbow_sensor, claw_sensor, wrist_sensor, bicep_sensor, allen_key_sensor, turntable_sensor };

void initCAN() {
  // will need new id's
  can.filter(ROVER_CANID_FIRST_SAFETY_RX, ROVER_CANID_FILTER_MASK, CANStandard);
}

// TODO see https://github.com/uwrobotics/MarsRover2020-firmware/blob/master/apps/arm/main.cpp for better implemetation
// to handle CAN messages set alert limit register for different current sensors
static mbed_error_status_t setAlertLimit(CANMsg &msg) {
  float alert_limit_value;
  msg.getPayload(alert_limit_value);

  switch (msg.id) {
    case CANID::SET_ELBOW_ALERT_LIMIT:
      /* code */
      break;
    case CANID::SET_CLAW_ALERT_LIMIT:
      /* code */
      break;
    case CANID::SET_BICEP_ALERT_LIMIT:
      /* code */
      break;
    case CANID::SET_WRIST_ALERT_LIMIT:
      /* code */
      break;
    case CANID::SET_ALLEN_KEY_ALERT_LIMIT:
      /* code */
      break;
    case CANID::SET_TURNTABLE_ALERT_LIMIT:
      /* code */
      break;
    default:
      return MBED_ERROR_INVALID_ARGUMENT;
  }
}

// set configuration for different current sensors
static mbed_error_status_t setConfiguration(CANMsg &msg) {
  float alert_limit_value;
  msg.getPayload(alert_limit_value);

  switch (msg.id) {
    case CANID::SET_ELBOW_CONFIGURATION:
      /* code */
      break;
    case CANID::SET_CLAW_CONFIGURATION:
      /* code */
      break;
    case CANID::SET_BICEP_CONFIGURATION:
      /* code */
      break;
    case CANID::SET_WRIST_CONFIGURATION:
      /* code */
      break;
    case CANID::SET_ALLEN_KEY_CONFIGURATION:
      /* code */
      break;
    case CANID::SET_TURNTABLE_CONFIGURATION:
      /* code */
      break;
    default:
      return MBED_ERROR_INVALID_ARGUMENT;
  }
}

static CANMsg::CANMsgHandlerMap canHandlerMap = {
    {CANID::SET_ELBOW_ALERT_LIMIT, &setAlertLimit},          {CANID::SET_CLAW_ALERT_LIMIT, &setAlertLimit},
    {CANID::SET_BICEP_ALERT_LIMIT, &setAlertLimit},          {CANID::SET_WRIST_ALERT_LIMIT, &setAlertLimit},
    {CANID::SET_ALLEN_KEY_ALERT_LIMIT, &setAlertLimit},      {CANID::SET_TURNTABLE_ALERT_LIMIT, &setAlertLimit},
    {CANID::SET_ELBOW_CONFIGURATION, &setConfiguration},     {CANID::SET_CLAW_CONFIGURATION, &setConfiguration},
    {CANID::SET_BICEP_CONFIGURATION, &setConfiguration},     {CANID::SET_WRIST_CONFIGURATION, &setConfiguration},
    {CANID::SET_ALLEN_KEY_CONFIGURATION, &setConfiguration}, {CANID::SET_TURNTABLE_CONFIGURATION, &setConfiguration}};

// send data/alerts back to the controller
// void txCANProcessor()
// {
//     CANMsg tsMsg;

//     struct __attribute__((__packed__)) safetyAlerts
//     {
//         float current;
//         float voltage;
//         char* sensor;
//     }safetyAlerts;

//     while(true)
//     {
//         txMsg.id = REPORT_ELBOW_SAFETY_DATA;
//         safetyAlerts.current = elbow_sensor.current;
//         safetyAlerts.voltage = elbow_sensor.voltage;
//         safetyAlerts.sensor = elbow_sensor.sensor_name;
//         txMsg.id.setPayload(safetyAlerts);
//         can.write(txMsg);
//         wait(1);

//         txMsg.id = REPORT_CLAW_SAFETY_DATA;
//         safetyAlerts.current = claw_sensor.current;
//         safetyAlerts.voltage = claw_sensor.voltage;
//         safetyAlerts.sensor = claw_sensor.sensor_name;
//         txMsg.id.setPayload(safetyAlerts);
//         can.write(txMsg);
//         wait(1);

//         txMsg.id = REPORT_WRIST_SAFETY_DATA;
//         safetyAlerts.current = wrist_sensor.current;
//         safetyAlerts.voltage = wrist_sensor.voltage;
//         safetyAlerts.sensor = wrist_sensor.sensor_name;
//         txMsg.id.setPayload(safetyAlerts);
//         can.write(txMsg);
//         wait(1);

//         txMsg.id = REPORT_BICEP_SAFETY_DATA;
//         safetyAlerts.current = bicep_sensor.current;
//         safetyAlerts.voltage = bicep_sensor.voltage;
//         safetyAlerts.sensor = bicep_sensor.sensor_name;
//         txMsg.id.setPayload(safetyAlerts);
//         can.write(txMsg);
//         wait(1);

//         txMsg.id = REPORT_ALLEN_KEY_SAFETY_DATA;
//         safetyAlerts.current = allen_key_sensor.current;
//         safetyAlerts.voltage = allen_key_sensor.voltage;
//         safetyAlerts.sensor = allen_key_sensor.sensor_name;
//         txMsg.id.setPayload(safetyAlerts);
//         can.write(txMsg);
//         wait(1);

//         txMsg.id = REPORT_TURNTABLE_SAFETY_DATA;
//         safetyAlerts.current = turntable_sensor.current;
//         safetyAlerts.voltage = turntable_sensor.voltage;
//         safetyAlerts.sensor = turntable_sensor.sensor_name;
//         txMsg.id.setPayload(safetyAlerts);
//         can.write(txMsg);
//         wait(1);

//         txMsg.id = REPORT_TEMP_DATA;
//         txMsg << thermistor.temp;
//         can.write(txMsg);
//     }
// }

// if in range returns 1, otherwise returns 0
bool inRange(float upper_lim, float lower_lim, float val) {
  return (val > upper_lim || val < lower_lim) ? 0 : 1;
}

void checkSafetySensors(INA_226 safety_sensors[]) {
  // TODO: implement hardware indication of danger, print to serial
  for (int safety_sensor = 0; safety_sensor < NUM_SAFETY_SENSORS; safety_sensor++) {
    float measured_current = safety_sensors[safety_sensor].getCurrentData();
    if (!inRange(CURRENT_UPPER_LIM, CURRENT_LOWER_LIM, measured_current)) {
      pc.printf("Measured %s Current is NOT within expected limits!\r\n",
                safety_sensors[safety_sensor].getSensorName());
    } else {
      pc.printf("Measured %s Current within expected limits!\r\n", safety_sensors[safety_sensor].getSensorName());
    }

    float measured_voltage = safety_sensors[safety_sensor].getVoltageData();
    if (!inRange(VOLTAGE_UPPER_LIM, VOLTAGE_LOWER_LIM, measured_voltage)) {
      pc.printf("Measured %s Voltage is NOT within expected limits!\r\n",
                safety_sensors[safety_sensor].getSensorName());
    } else {
      pc.printf("Measured %s Voltage within expected limits!\r\n", safety_sensors[safety_sensor].getSensorName());
    }
    // float measured_power = safety_sensors[sensor_index].getPowerData();
    // if (!inRange(POWER_UPPER_LIM, POWER_LOWER_LIM, measured_power))
    // {
    //     pc.printf("Measured Power is NOT within expected limits!\r\n");
    // }
  }
}

void calibrateSafetySensors(INA_226 safety_sensors[]) {
  for (int safety_sensor = 0; safety_sensor < NUM_SAFETY_SENSORS; safety_sensor++) {
    safety_sensors[safety_sensor].calibrateSensor();
    pc.printf("%s Calibrated\r\n", safety_sensors[safety_sensor].getSensorName());
  }
}

void checkThermistor(VoltageDividerThermistor temp_sensor) {
  float measured_temp = temp_sensor.readThermistorCelcius();
  if (!inRange(TEMP_UPPER_LIM, TEMP_LOWER_LIM, measured_temp)) {
    pc.printf("Measured Temperature is NOT within expected limits!\r\n");
  }
}

int main() {
  // current sensor data https://docs.google.com/spreadsheets/d/1Qn4QbEJ1Ia54vYnTgaTvLxHralQvCz6PEGaSF_O1L20/edit#gid=0
  // SDA Pin, SCL pin, sensor address, shunt resistance, max expected current
  // TODO: confirm values for sensors
  ComponentConfig elbow_config     = {"Elbow", CUR_SEN_I2C_SDA, CUR_SEN_I2C_SCL, 0x00, 0.003, 28.75};
  ComponentConfig claw_config      = {"Claw", CUR_SEN_I2C_SDA, CUR_SEN_I2C_SCL, 0x00, 0.012, 6.325};
  ComponentConfig wrist_config     = {"Wrist", CUR_SEN_I2C_SDA, CUR_SEN_I2C_SCL, 0x00, 0.01, 8.05};
  ComponentConfig bicep_config     = {"Bicep", CUR_SEN_I2C_SDA, CUR_SEN_I2C_SCL, 0x00, 0.015, 5.75};
  ComponentConfig allen_key_config = {"Allen Key", CUR_SEN_I2C_SDA, CUR_SEN_I2C_SCL, 0x00, 0.022, 3.45};
  ComponentConfig turntable_config = {"Turntable", CUR_SEN_I2C_SDA, CUR_SEN_I2C_SCL, 0x00, 0.00075, 60.95};

  INA_226 safety_sensors[] = {INA_226(elbow_config), INA_226(claw_config),      INA_226(wrist_config),
                              INA_226(bicep_config), INA_226(allen_key_config), INA_226(turntable_config)};

  calibrateSafetySensors(safety_sensors);

  // thermistor pin, B constant, vin, expected room temp, thermistor room temp resistance, voltage divider resistance
  VoltageDividerThermistorConfig safety_thermistor_config = {TEMP_ANLG_IN, 4700, 5, 295.15, 100000, 100000};
  VoltageDividerThermistor temp_sensor(safety_thermistor_config);

  initCAN();

  while (1) {
    // read can messages to get data to configure device
    // u_int8_t command = 0;
    // if(can.read(rxMsg))
    // {
    //     if (canHandlerMap.count(rxMsg.id) > 0)
    //     {
    //         canHandlerMap[rxMsg.id](rxMsg);
    //     }
    // }

    checkSafetySensors(safety_sensors);

    checkThermistor(temp_sensor);
  }
  return 1;
}