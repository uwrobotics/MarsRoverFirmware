#include "PDBMonitoring.h"

#include <cmath>

#include "Logger.h"
#include "CANConfig.h"
#include "hw_bridge.h"

const float PDBMonitoring::PDB_VBAT_RAIL_NOMINAL_VOLTAGE = 48.0;
const float PDBMonitoring::PDB_VBAT_RAIL_MIN_THRESHOLD   = 18.0;
const float PDBMonitoring::PDB_VBAT_RAIL_MAX_THRESHOLD   = 25.2;

const float PDBMonitoring::PDB_17V_RAIL_NOMINAL_VOLTAGE = 17.0;
const float PDBMonitoring::PDB_17V_RAIL_MIN_THRESHOLD   = 16.0;
const float PDBMonitoring::PDB_17V_RAIL_MAX_THRESHOLD   = 18.0;

const float PDBMonitoring::PDB_5V_RAIL_NOMINAL_VOLTAGE = 5.0;
const float PDBMonitoring::PDB_5V_RAIL_MIN_THRESHOLD   = 4.8;
const float PDBMonitoring::PDB_5V_RAIL_MAX_THRESHOLD   = 6.0;

const float PDBMonitoring::PDB_TEMPERATURE_MIN_THRESHOLD = 10.0;
const float PDBMonitoring::PDB_TEMPERATURE_MAX_THRESHOLD = 50.0;

HWBRIDGE::PDB_LED_MATRIX_STATE_VALUES LED_matrix_status   = HWBRIDGE::PDB_LED_MATRIX_STATE_VALUES::False;

CANInterface can(CANConfig::config);

PDBMonitoring::PDBMonitoring()
    : load1_5V_diag_en(LOAD1_5V_DIAG_EN),
      load1_5V_fault_n(LOAD1_5V_FAULT),
      load2_5V_diag_en(LOAD2_5V_DIAG_EN),
      load2_5V_fault_n(LOAD2_5V_FAULT),
      load3_5V_diag_en(LOAD3_5V_DIAG_EN),
      load3_5V_fault_n(LOAD3_5V_FAULT),
      load4_5V_diag_en(LOAD4_5V_DIAG_EN),
      load4_5V_fault_n(LOAD4_5V_FAULT),
      load_17V_diag_en(LOAD_17V_DIAG_EN),
      load_17V_fault_n(LOAD_17V_FAULT),
      railBattery(RAIL_BATTERY_ANLG_IN),
      rail5V(RAIL_5V_ANLG_IN),
      rail17V(RAIL_17V_ANLG_IN),
      rail24V(RAIL_24V_ANLG_IN),
      rail24V_pgood_n(RAIL_24V_PGOOD_N),
      temperatureADC(TEMPERATURE_ADC_IN),
      LED_matrix_red(LED_MATRIX_R_CHANNEL),
      LED_matrix_blue(LED_MATRIX_B_CHANNEL),
      LED_matrix_green(LED_MATRIX_G_CHANNEL),
  {
  load1_5V_diag_en.write(1);
  load2_5V_diag_en.write(1);
  load3_5V_diag_en.write(1);
  load4_5V_diag_en.write(1);
  load_17V_diag_en.write(1);
}

/*TODO:Replace the logger statements with CAN logs
 * after CAN Module is ready */

void PDBMonitoring::load_monitoring() {
  if (load1_5V_diag_en && !load1_5V_fault_n) {
    load1_5V_diag_en.write(0);
    Utility::logger << "Fault on 5V load 1\n";
  }else{
    load1_5V_diag_en.write(1);
  }
  if (load2_5V_diag_en && !load2_5V_fault_n) {
    Utility::logger << "Fault on 5V load 2\n";
    load2_5V_diag_en.write(0);
  }else{
    load2_5V_diag_en.write(1);
  }
  if (load3_5V_diag_en && !load3_5V_fault_n) {
    Utility::logger << "Fault on 5V load 3\n";
    load3_5V_diag_en.write(0);
  }else{
    load3_5V_diag_en.write(1);
  }
  if (load4_5V_diag_en && !load4_5V_fault_n) {
    Utility::logger << "Fault on 5V load 4\n";
    load4_5V_diag_en.write(0);
  }else{
    load4_5V_diag_en.write(1);
  }
  if (load_17V_diag_en && !load_17V_fault_n) {
    Utility::logger << "Fault on 17V load\n";
    load_17V_diag_en.write(0);
  }else{
    load_17V_diag_en.write(1);
  }
}

void PDBMonitoring::rail_monitoring() {
  float rail_vbat_voltage = railBattery.read_voltage() / 3 * PDB_VBAT_RAIL_NOMINAL_VOLTAGE;
  float rail_17V_voltage  = rail17V.read_voltage() / 3 * PDB_17V_RAIL_NOMINAL_VOLTAGE;
  float rail_5V_voltage   = rail5V.read_voltage() / 3 * PDB_5V_RAIL_NOMINAL_VOLTAGE;

  if (rail_vbat_voltage < PDB_VBAT_RAIL_MIN_THRESHOLD || rail_vbat_voltage > PDB_VBAT_RAIL_MAX_THRESHOLD) {
    Utility::logger << "!!! VBAT RAIL VOLTAGE: %.3fV !!!\n", rail_vbat_voltage;
  }
  if (rail_17V_voltage < PDB_17V_RAIL_MIN_THRESHOLD || rail_17V_voltage > PDB_17V_RAIL_MAX_THRESHOLD) {
    Utility::logger << "!!! 17V RAIL VOLTAGE: %.3fV !!!\n", rail_17V_voltage;
  }
  if (rail_5V_voltage < PDB_5V_RAIL_MIN_THRESHOLD || rail_5V_voltage > PDB_5V_RAIL_MAX_THRESHOLD) {
    Utility::logger << "!!! 5V RAIL VOLTAGE: %.3fV !!!\n", rail_5V_voltage;
  }

  // Sends can status messaging
  //TODO: Make CAN sending work
  if(rail_17V_voltage < PDB_17V_RAIL_MIN_THRESHOLD){
    can.setTXSignalValue(HWBRIDGE::CANID::PDB_REPORT_FAULTS, HWBRIDGE::CANSIGNAL::PDB_17_V_RAIL_STATUS_VALUES,
                         HWBRIDGE::CANSIGNAL::PDB_17_V_RAIL_STATUS_VALUES::UNDERVOLTAGE);
  }else if(rail_17V_voltage > PDB_17V_RAIL_MAX_THRESHOLD){
    can.setTXSignalValue(HWBRIDGE::CANID::PDB_REPORT_FAULTS, HWBRIDGE::CANSIGNAL::PDB_17_V_RAIL_STATUS_VALUES,
                         HWBRIDGE::CANSIGNAL::PDB_17_V_RAIL_STATUS_VALUES::OVERVOLTAGE);
  }else{
    can.setTXSignalValue(HWBRIDGE::CANID::PDB_REPORT_FAULTS, HWBRIDGE::CANSIGNAL::PDB_17_V_RAIL_STATUS_VALUES,
                         HWBRIDGE::CANSIGNAL::PDB_17_V_RAIL_STATUS_VALUES::OPERATIONAL);
  }

  if(rail_5V_voltage < PDB_5V_RAIL_MIN_THRESHOLD){
    can.setTXSignalValue(HWBRIDGE::CANID::PDB_REPORT_FAULTS, HWBRIDGE::CANSIGNAL::PDB_5_V_RAIL_STATUS_VALUES,
                         HWBRIDGE::CANSIGNAL::PDB_5_V_RAIL_STATUS_VALUES::UNDERVOLTAGE);
  }else if(rail_5V_voltage > PDB_5V_RAIL_MAX_THRESHOLD){
    can.setTXSignalValue(HWBRIDGE::CANID::PDB_REPORT_FAULTS, HWBRIDGE::CANSIGNAL::PDB_5_V_RAIL_STATUS_VALUES,
                         HWBRIDGE::CANSIGNAL::PDB_5_V_RAIL_STATUS_VALUES::OVERVOLTAGE);
  }else{
    can.setTXSignalValue(HWBRIDGE::CANID::PDB_REPORT_FAULTS, HWBRIDGE::CANSIGNAL::PDB_5_V_RAIL_STATUS_VALUES,
                         HWBRIDGE::CANSIGNAL::PDB_5_V_RAIL_STATUS_VALUES::OPERATIONAL);

  }
}
void PDBMonitoring::LED_matrix(){
  //TODO: Make CAN receive work
  can.getRXSignalValue(HWBRIDGE::CANID::PDB_SET_LED_MATRIX, HWBRIDGE::CANSIGNAL::PDB_LED_MATRIX_STATE, LED_matrix_status);
  if(LED_matrix_status == HWBRIDGE::PDB_LED_MATRIX_STATE_VALUES::SOLID_RED){
    LED_matrix_red.write(1);
    LED_matrix_green.write(0);
    LED_matrix_blue.write(0);
  }else if(LED_matrix_status == HWBRIDGE::PDB_LED_MATRIX_STATE_VALUES::FLASHING_GREEN){
    LED_matrix_green.write(0.50f);
    LED_matrix_blue.write(0);
    LED_matrix_red.write(0);
  }else if(LED_matrix_status == HWBRIDGE::PDB_LED_MATRIX_STATE_VALUES::SOLID_BLUE){
    LED_matrix_blue.write(1);
    LED_matrix_red.write(0);
    LED_matrix_green.write(0);
  }else{
    LED_matrix_blue.write(0);
    LED_matrix_red.write(0);
    LED_matrix_green.write(0);
  }
}

/*https://www.ti.com/lit/ds/symlink/lmt87-q1.pdf?ts=1627158177761&ref_url=https%253A%252F%252Fwww.google.com%252F*/

// void PDBMonitoring::temperature_monitoring() {
//   float temperature_celsius =
//       (13.582 - sqrt(pow(-13.582, 2) + 4 * 0.00433 * (2230.8 - temperatureADC.read_voltage() * 1000))) /
//           (2 * (-0.00433)) +
//       30;

//   if (temperature_celsius < PDB_TEMPERATURE_MIN_THRESHOLD || temperature_celsius > PDB_TEMPERATURE_MAX_THRESHOLD) {
//     Utility::logger << "!!! TEMPERATURE FAULT:" << temperature_celsius << "degrees Celsius\n";
//   }
// }

void PDBMonitoring::periodic_1s() {
  // temperature_monitoring();
  load_monitoring();
  LED_matrix();
}

void PDBMonitoring::periodic_1ms() {
  /*Monitoring Period = 500ms*/
  int rail_counter = 0;

  if (rail_counter % 5 == 0) {
    rail_monitoring();
    rail_counter = 0;
  }

  rail_counter++;
}
