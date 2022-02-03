#include "BMSMonitoring.h"

#include <cmath>

#include "Logger.h"

BMSMonitoring::BMSMonitoring() {
    buzzer_en(BUZZER_EN);
    currentADC(CURR_ANLG_IN);
    spi(SDI, SDO, CLK); // mosi, miso, sclk
    chip_select(SAMPL);
    chip_select = 1;
    spi.format(24,3); 
    spi.frequency(1000000);
}

void BMSMonitoring::cell_monitoring() {
  chip_select = 0;
  //is_cell_balancing = getRXSignal
  int spi_message = 0b000000000000000000000000;
  if(is_cell_balancing){
    spi_message = 0b111111111111111100000000;
  }

  for(int i = 0; i < 16; i++){
    cell_voltages[i] = spi.write(spi_message);
    spi_message += 0b10000;
  }
  //setTXSignalValue(HWBRIDGE::CANID msgID, HWBRIDGE::CANSIGNAL signalName, HWBRIDGE::CANSignalValue_t signalValue);


  //if voltage is too low on one cell, enable balancing on that cell
  //only way to balance is to keep all fets on so cells always have same voltage
}

void BMSMonitoring::current_monitoring() {

  float current = (currentADC.read_voltage() * 20) -33; 
  if(current < 0){
    current = 0;
  }else if(current > 29){
    current = 29.1;
  }
  current = current * 1000; //converts Amps to MilliAmps
  coloumb_count += current * 0.001; //converts to MilliAmp Seconds
  int battery_capacity_seconds = battery_capacity * 3600; //converts to MilliAmp Seconds

  battery_percent = (1 - (coloumb_count / battery_capacity_seconds)) * 100;

  if(battery_percent < 20){
    buzzer_en = true;
  }else{
    buzzer_en = false;
  }
}

void BMSMonitoring::periodic_1s() {
  
}

void BMSMonitoring::periodic_1ms() {
  current_monitoring();
}


//TODO:
// make it work with CAN
// voltage look-up table