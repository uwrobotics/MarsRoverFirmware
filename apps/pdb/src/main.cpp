#include <cmath>

#include "CANBus.h"
#include "CANMsg.h"
#include "Logger.h"
#include "PDB_config.h"
#include "hw_bridge.h"

// Rail Monitoring
AnalogIn railBattery(RAIL_BATTERY_ANLG_IN);
AnalogIn rail5V(RAIL_5V_ANLG_IN);
AnalogIn rail17V(RAIL_17V_ANLG_IN);
AnalogIn rail24V(RAIL_24V_ANLG_IN);
DigitalIn rail24V_pgood_n(RAIL_24V_PGOOD_N);
// TODO: add voltage range (as percentage) to hw bridge also allocate can id for reporting if outside range

// Load Monitoring
DigitalOut load1_5V_diag_en(LOAD1_5V_DIAG_EN);
DigitalIn load1_5V_fault_n(LOAD1_5V_FAULT);

DigitalOut load2_5V_diag_en(LOAD2_5V_DIAG_EN);
DigitalIn load2_5V_fault_n(LOAD2_5V_FAULT);

DigitalOut load3_5V_diag_en(LOAD3_5V_DIAG_EN);
DigitalIn load3_5V_fault_n(LOAD3_5V_FAULT);

DigitalOut load4_5V_diag_en(LOAD4_5V_DIAG_EN);
DigitalIn load4_5V_fault_n(LOAD4_5V_FAULT);

DigitalOut load5_5V_diag_en(LOAD5_5V_DIAG_EN);
DigitalIn load5_5V_fault_n(LOAD5_5V_FAULT);

DigitalOut load_17V_diag_en(LOAD_17V_DIAG_EN);
DigitalIn load_17V_fault_n(LOAD_17V_FAULT);

// Temperature Monitoring
AnalogIn temperatureADC(TEMPERATURE_ADC_IN);

// CAN
CANBus can1(CAN1_RX, CAN1_TX, HWBRIDGE::ROVERCONFIG::ROVER_CANBUS_FREQUENCY);

// Threads
Thread rxCANProcessorThread(osPriorityAboveNormal);
Thread txCANProcessorThread(osPriorityBelowNormal);
Thread loadMonitoringThread(osPriorityAboveNormal);
Thread railMonitoringThread(osPriorityAboveNormal);
Thread temperatureMonitoringThread(osPriorityAboveNormal);

// Function prototypes
void rxCANProcessor();
void txCANProcessor();
void loadMonitoring();
void railMonitoring();
void temperatureMonitoring();

// simple switch statement that calls a different function based on contents of CAN msg
static mbed_error_status_t setLEDMatrix(CANMsg &msg);

static float adcToRailVoltage(float adc_voltage, float rail_nominal_voltage);
static bool valueOutOfRange(float value, float lower_threshold, float upper_threshold);

void PDB_init();

// TODO: rename NEOPIXEL_SET to LEDMATRIX_SET
const static CANMsg::CANMsgHandlerMap canHandlerMap = {{HWBRIDGE::CANID::NEOPIXEL_SET, &setLEDMatrix}};

int main() {
  Utility::Logger::printf("===== PDB App =====\n\n");
  PDB_init();
  ThisThread::sleep_for(1ms);

  rxCANProcessorThread.start(rxCANProcessor);
  txCANProcessorThread.start(txCANProcessor);
  loadMonitoringThread.start(loadMonitoring);
  railMonitoringThread.start(railMonitoring);
  temperatureMonitoringThread.start(temperatureMonitoring);

  while (true) {
    ThisThread::sleep_for(1000ms);
  }
}

void rxCANProcessor() {
  CANMsg rxMsg;
  while (true) {
    if (can1.read(rxMsg)) {
      canHandlerMap.at(rxMsg.getID())(rxMsg);
    }
    ThisThread::sleep_for(PDB_RX_CAN_PROCESSING_PERIOD);
  }
}

void txCANProcessor() {
  CANMsg txMsg;
  while (true) {
    // TODO: stream ultrasonic data
    ThisThread::sleep_for(PDB_TX_CAN_PROCESSING_PERIOD);
  }
}

static mbed_error_status_t setLEDMatrix(CANMsg &msg) {
  return MBED_ERROR_INVALID_ARGUMENT;
}

void PDB_init() {
  // Smart switch diagnostic enables
  load1_5V_diag_en = PDB_5V_LOAD1_DIAG_EN;
  load2_5V_diag_en = PDB_5V_LOAD2_DIAG_EN;
  load3_5V_diag_en = PDB_5V_LOAD3_DIAG_EN;
  load4_5V_diag_en = PDB_5V_LOAD4_DIAG_EN;
  load5_5V_diag_en = PDB_5V_LOAD5_DIAG_EN;
  load_17V_diag_en = PDB_17V_LOAD_DIAG_EN;
}

// Raises alert in case of overcurrent (>5A) or open load
void loadMonitoring() {
  while (true) {
    if (load1_5V_diag_en && !load1_5V_fault_n) {
      Utility::Logger::printf("Fault on 5V load 1\n");
    }
    if (load2_5V_diag_en && !load2_5V_fault_n) {
      Utility::Logger::printf("Fault on 5V load 2\n");
    }
    if (load3_5V_diag_en && !load3_5V_fault_n) {
      Utility::Logger::printf("Fault on 5V load 3\n");
    }
    if (load4_5V_diag_en && !load4_5V_fault_n) {
      Utility::Logger::printf("Fault on 5V load 4\n");
    }
    if (load5_5V_diag_en && !load5_5V_fault_n) {
      Utility::Logger::printf("Fault on 5V load 5\n");
    }
    if (load_17V_diag_en && !load_17V_fault_n) {
      Utility::Logger::printf("Fault on 17V load\n");
    }

    ThisThread::sleep_for(PDB_LOAD_MONITORING_PERIOD);
  }
}

void railMonitoring() {
  while (true) {
    float rail_vbat_voltage = adcToRailVoltage(railBattery.read_voltage(), PDB_VBAT_RAIL_NOMINAL_VOLTAGE);
    float rail_24V_voltage  = adcToRailVoltage(rail24V.read_voltage(), PDB_24V_RAIL_NOMINAL_VOLTAGE);
    float rail_17V_voltage  = adcToRailVoltage(rail17V.read_voltage(), PDB_17V_RAIL_NOMINAL_VOLTAGE);
    float rail_5V_voltage   = adcToRailVoltage(rail5V.read_voltage(), PDB_5V_RAIL_NOMINAL_VOLTAGE);

    if (valueOutOfRange(rail_vbat_voltage, PDB_VBAT_RAIL_MIN_THRESHOLD, PDB_VBAT_RAIL_MAX_THRESHOLD)) {
      Utility::Logger::printf("!!! VBAT RAIL VOLTAGE: %.3fV !!!\n", rail_vbat_voltage);
    }
    if (valueOutOfRange(rail_24V_voltage, PDB_24V_RAIL_MIN_THRESHOLD, PDB_24V_RAIL_MAX_THRESHOLD)) {
      Utility::Logger::printf("!!! 24V RAIL VOLTAGE: %.3fV !!!\n", rail_24V_voltage);
    }
    if (valueOutOfRange(rail_17V_voltage, PDB_17V_RAIL_MIN_THRESHOLD, PDB_17V_RAIL_MAX_THRESHOLD)) {
      Utility::Logger::printf("!!! 17V RAIL VOLTAGE: %.3fV !!!\n", rail_17V_voltage);
    }
    if (valueOutOfRange(rail_5V_voltage, PDB_5V_RAIL_MIN_THRESHOLD, PDB_5V_RAIL_MAX_THRESHOLD)) {
      Utility::Logger::printf("!!! 5V RAIL VOLTAGE: %.3fV !!!\n", rail_5V_voltage);
    }

    ThisThread::sleep_for(PDB_RAIL_MONITORING_PERIOD);
  }
}

// Temperature sensor datasheet: https://www.ti.com/lit/ds/symlink/lmt87-q1.pdf?src-supplier=IHS+Markit
void temperatureMonitoring() {
  while (true) {
    float temperature_celsius =
        (13.582 - sqrt(pow(-13.582, 2) + 4 * 0.00433 * (2230.8 - temperatureADC.read_voltage() * 1000))) /
            (2 * (-0.00433)) +
        30;

    if (valueOutOfRange(temperature_celsius, PDB_TEMPERATURE_MIN_THRESHOLD, PDB_TEMPERATURE_MAX_THRESHOLD)) {
      Utility::Logger::printf("!!! TEMPERATURE FAULT: %.3f degrees Celsius\n", temperature_celsius);
    }

    ThisThread::sleep_for(PDB_TEMPERATURE_MONITORING_PERIOD);
  }
}

// Helpers

static float adcToRailVoltage(float adc_voltage, float rail_nominal_voltage) {
  return adc_voltage / 3 * rail_nominal_voltage;  // Voltage divider to 3V
}

static bool valueOutOfRange(float value, float lower_threshold, float upper_threshold) {
  return ((value < lower_threshold) || (value > upper_threshold));
}
