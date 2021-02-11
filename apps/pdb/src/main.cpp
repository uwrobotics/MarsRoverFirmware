#include <cmath>

#include "CANBus.h"
#include "CANMsg.h"
<<<<<<< HEAD
#include "Logger.h"
=======
#include "PDB_config.h"
>>>>>>> Add rail and load monitoring
#include "hw_bridge.h"

// Logger
Utility::Logger logger;

// Rail Monitoring
AnalogIn railBattery(RAIL_BATTERY_ANLG_IN), rail5V(RAIL_5V_ANLG_IN), rail17V(RAIL_17V_ANLG_IN),
    rail24V(RAIL_24V_ANLG_IN);  // add voltage range (as percentage) to hw bridge also allocate can id for reporting if
                                // outside range

// Load Monitoring
DigitalOut load5V_1_diag_en(SMART_SWITCH_5V_LOAD1_DIAG_EN);
DigitalIn load5V_1_diag_fault_n(SMART_SWITCH_5V_LOAD1_FAULT);

DigitalOut load5V_2_diag_en(SMART_SWITCH_5V_LOAD2_DIAG_EN);
DigitalIn load5V_2_diag_fault_n(SMART_SWITCH_5V_LOAD2_FAULT);

DigitalOut load5V_3_diag_en(SMART_SWITCH_5V_LOAD3_DIAG_EN);
DigitalIn load5V_3_diag_fault_n(SMART_SWITCH_5V_LOAD3_FAULT);

DigitalOut load5V_4_diag_en(SMART_SWITCH_5V_LOAD4_DIAG_EN);
DigitalIn load5V_4_diag_fault_n(SMART_SWITCH_5V_LOAD4_FAULT);

DigitalOut load5V_5_diag_en(SMART_SWITCH_5V_LOAD5_DIAG_EN);
DigitalIn load5V_5_diag_fault_n(SMART_SWITCH_5V_LOAD5_FAULT);

DigitalOut load17V_diag_en(SMART_SWITCH_17V_LOAD_DIAG_EN);
DigitalIn load17V_diag_fault_n(SMART_SWITCH_17V_LOAD_FAULT);

// CAN
CANBus can1(CAN1_RX, CAN1_TX, HWBRIDGE::ROVERCONFIG::ROVER_CANBUS_FREQUENCY);

// Threads
Thread rxCANProcessorThread(osPriorityAboveNormal);
Thread txCANProcessorThread(osPriorityBelowNormal);
Thread loadMonitoringProcessorThread(osPriorityBelowNormal);
Thread railMonitoringProcessorThread(osPriorityBelowNormal);

// Function prototypes

void rxCANProcessor();
void txCANProcessor();
void loadMonitoringProcessor();
void railMonitoringProcessor();

static mbed_error_status_t setLEDMatrix(
    CANMsg &msg);  // simple switch statement that calls a different function based on contents of CAN msg

static float adcToRailVoltage(float adc_voltage, float rail_nominal_voltage);
static bool valueOutOfRange(float value, float lower_threshold, float upper_threshold);

void PDB_init();

const static CANMsg::CANMsgHandlerMap canHandlerMap = {
    {HWBRIDGE::CANID::NEOPIXEL_SET, &setLEDMatrix}};  // TODO: rename NEOPIXEL_SET to LEDMATRIX_SET

int main() {
  PDB_init();
  ThisThread::sleep_for(1ms);

  rxCANProcessorThread.start(rxCANProcessor);
  txCANProcessorThread.start(txCANProcessor);
  loadMonitoringProcessorThread.start(loadMonitoringProcessor);
  railMonitoringProcessorThread.start(railMonitoringProcessor);

  while (true)
    ;
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
  load5V_1_diag_en = PDB_5V_LOAD1_ENABLE;
  load5V_2_diag_en = PDB_5V_LOAD2_ENABLE;
  load5V_3_diag_en = PDB_5V_LOAD3_ENABLE;
  load5V_4_diag_en = PDB_5V_LOAD4_ENABLE;
  load5V_5_diag_en = PDB_5V_LOAD5_ENABLE;
  load17V_diag_en  = PDB_17V_LOAD_ENABLE;
}

void loadMonitoringProcessor() {
  while (true) {
    if (load5V_1_diag_fault_n) {
      logger << "Fault on 5V load 1\n";
    }
    if (load5V_2_diag_fault_n) {
      logger << "Fault on 5V load 2\n";
    }
    if (load5V_3_diag_fault_n) {
      logger << "Fault on 5V load 3\n";
    }
    if (load5V_4_diag_fault_n) {
      logger << "Fault on 5V load 4\n";
    }
    if (load5V_5_diag_fault_n) {
      logger << "Fault on 5V load 5\n";
    }
    if (load17V_diag_fault_n) {
      logger << "Fault on 17V load\n";
    }
    ThisThread::sleep_for(PDB_LOAD_MONITORING_PERIOD);
  }
}

void railMonitoringProcessor() {
  while (true) {
    float rail_vbat_rail_voltage = adcToRailVoltage(railBattery.read_voltage(), PDB_VBAT_RAIL_NOMINAL_VOLTAGE);
    float rail_24V_rail_voltage  = adcToRailVoltage(rail24V.read_voltage(), PDB_24V_RAIL_NOMINAL_VOLTAGE);
    float rail_17V_rail_voltage  = adcToRailVoltage(rail17V.read_voltage(), PDB_17V_RAIL_NOMINAL_VOLTAGE);
    float rail_5V_rail_voltage   = adcToRailVoltage(rail5V.read_voltage(), PDB_5V_RAIL_NOMINAL_VOLTAGE);

    if (valueOutOfRange(rail_vbat_rail_voltage, PDB_VBAT_RAIL_MIN_THRESHOLD, PDB_VBAT_RAIL_MAX_THRESHOLD)) {
      logger << "!!! VBAT RAIL VOLTAGE: %0.3fV !!!" << rail_vbat_rail_voltage << "\n";
    }
    if (valueOutOfRange(rail_24V_rail_voltage, PDB_24V_RAIL_MIN_THRESHOLD, PDB_24V_RAIL_MAX_THRESHOLD)) {
      logger << "!!! 24V RAIL VOLTAGE: %0.3fV !!!" << rail_24V_rail_voltage << "\n";
    }
    if (valueOutOfRange(rail_17V_rail_voltage, PDB_17V_RAIL_MIN_THRESHOLD, PDB_17V_RAIL_MAX_THRESHOLD)) {
      logger << "!!! 17V RAIL VOLTAGE: %0.3fV !!!" << rail_17V_rail_voltage << "\n";
    }
    if (valueOutOfRange(rail_5V_rail_voltage, PDB_5V_RAIL_MIN_THRESHOLD, PDB_5V_RAIL_MAX_THRESHOLD)) {
      logger << "!!! 5V RAIL VOLTAGE: %0.3fV !!!" << rail_5V_rail_voltage << "\n";
    }
    ThisThread::sleep_for(PDB_RAIL_MONITORING_PERIOD);
  }
}

// Helpers

static float adcToRailVoltage(float adc_voltage, float rail_nominal_voltage) {
  return adc_voltage / 3 * rail_nominal_voltage;  // Voltage divider to 3V
}

static bool valueOutOfRange(float value, float lower_threshold, float upper_threshold) {
  return ((value < lower_threshold) || (value > upper_threshold));
}
