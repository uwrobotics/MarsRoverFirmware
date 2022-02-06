#include "BMSMonitoring.h"

#include <cmath>

#include "Logger.h"
#include "mbed.h"
#include <map>

SPI spi(SDI, SDO, CLK); // mosi, miso, sclk
AnalogIn currentADC(CURR_ANLG_IN);
DigitalOut buzzer_en(BUZZER_EN);
DigitalOut chip_select(SAMPL);

BMSMonitoring::BMSMonitoring() {
  buzzer_en.write(0);
  chip_select.write(1);
  spi.format(24,3); 
  spi.frequency(1000000);
}

void BMSMonitoring::cell_monitoring() {
  chip_select.write(0);

  //TODO: recieve data from can
  //is_cell_balancing = getRXSignal
  // can.getRXSignalValue(HWBRIDGE::CANID::BMS, HWBRIDGE::CANSIGNAL::BMS_STATE, is_cell_balancing);

  //if voltage is too low on one cell, enable balancing on that cell
  //only way to balance is to keep all fets on so cells always have same voltage
  int spi_message = 0b000000000000000000000000;
  if(is_cell_balancing){
    spi_message = 0b111111111111111100000000;
  }

  battery_voltage = 0;
  for(int i = 0; i < 16; i++){
    cell_voltages[i] = spi.write(spi_message);
    battery_voltage += cell_voltages[i];
    spi_message += 0b10000;
  }
  chip_select.write(1);

  update_voltage_percent();
}

void BMSMonitoring::update_voltage_percent() {
  if(battery_voltage >= 50.40){
    battery_percent_voltage = 100;
  }else if(battery_voltage >= 49.80){
    battery_percent_voltage = 95;
  }else if(battery_voltage >= 49.32){
    battery_percent_voltage = 90;
  }else if(battery_voltage >= 48.96){
    battery_percent_voltage = 85;
  }else if(battery_voltage >= 48.24){
    battery_percent_voltage = 80;
  }else if(battery_voltage >= 47.76){
    battery_percent_voltage = 75;
  }else if(battery_voltage >= 47.40){
    battery_percent_voltage = 70;
  }else if(battery_voltage >= 46.92){
    battery_percent_voltage = 65;
  }else if(battery_voltage >= 46.44){
    battery_percent_voltage = 60;
  }else if(battery_voltage >= 46.20){
    battery_percent_voltage = 55;
  }else if(battery_voltage >= 46.08){
    battery_percent_voltage = 50;
  }else if(battery_voltage >= 45.84){
    battery_percent_voltage = 45;
  }else if(battery_voltage >= 45.60){
    battery_percent_voltage = 40;
  }else if(battery_voltage >= 45.48){
    battery_percent_voltage = 35;
  }else if(battery_voltage >= 45.24){
    battery_percent_voltage = 30;
  }else if(battery_voltage >= 45.00){
    battery_percent_voltage = 25;
  }else if(battery_voltage >= 44.76){
    battery_percent_voltage = 20;
  }else if(battery_voltage >= 44.52){
    battery_percent_voltage = 15;
  }else if(battery_voltage >= 44.28){
    battery_percent_voltage = 10;
  }else if(battery_voltage >= 43.32){
    battery_percent_voltage = 5;
  }else{
    battery_percent_voltage = 0;
  }

}

void BMSMonitoring::current_monitoring() {
  float current = (currentADC.read() * 20) -33; 
  if(current < 0){
    current = 0;
  }else if(current > 29){
    current = 29.1;
  }
  current = current * 1000; //converts Amps to MilliAmps
  coloumb_count += current * 0.001; //converts to MilliAmp Seconds
  int battery_capacity_seconds = battery_capacity * 3600; //converts to MilliAmp Seconds
  battery_percent_couloumb = (1 - (coloumb_count / battery_capacity_seconds)) * 100;
}

void BMSMonitoring::low_battery_warning(){
  if(battery_percent_couloumb <= 15 || battery_percent_voltage <= 15){
    buzzer_en.write(1);
  }else{
    buzzer_en.write(0);
  }
}

void BMSMonitoring::send_can_data(){
  //TODO: Send data over CAN
  // Things to send over CAN
  // - cell voltages for 12 cells: cell_voltages
  // - percent battery: battery_percent_couloumb and battery_percent_couloumb
  // - current: current
  //setTXSignalValue(HWBRIDGE::CANID msgID, HWBRIDGE::CANSIGNAL signalName, HWBRIDGE::CANSignalValue_t signalValue);

}

void BMSMonitoring::periodic_1s() {
  low_battery_warning();
  send_can_data();
}

void BMSMonitoring::periodic_1ms() {
  current_monitoring();
  cell_monitoring();

}
