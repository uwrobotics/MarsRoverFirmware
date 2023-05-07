#include "BMSMonitoring.h"
#include <iostream>

#include <cmath>
#include <map>

#include "Logger.h"
#include "mbed.h"
#include "PinNames.h"

#include "PinNamesTypes.h"
#include "cmsis.h"

SPI m_spi(SDI, SDO, CLK);  // mosi, miso, sclk
AnalogIn m_currentADC(CURR_ANLG_IN);
DigitalOut m_buzzer_en(BUZZER_EN);
DigitalOut m_chip_select(SAMPL);

BMSMonitoring::BMSMonitoring() {
  m_buzzer_en.write(0);
  m_chip_select.write(1);
  m_spi.format(m_spi_bits, m_spi_mode);
  m_spi.frequency(m_spi_frequency);
}

void BMSMonitoring::cell_monitoring() {
  m_chip_select.write(0);

  // TODO: recieve data from can
  // is_cell_balancing = getRXSignal
  // can.getRXSignalValue(HWBRIDGE::CANID::BMS, HWBRIDGE::CANSIGNAL::BMS_STATE, is_cell_balancing);

  // if voltage is too low on one cell, enable balancing on that cell
  // only way to balance is to keep all fets on so cells always have same voltage
  int spi_message = (m_is_cell_balancing ? 0b111111111111111100000000 : 0b000000000000000000000000);

  m_battery_voltage = 0;
  for (int i = 0; i < 12; i++) {
    m_cell_voltages[i] = m_spi.write(spi_message);
    m_battery_voltage += m_cell_voltages[i];
    spi_message += 0b10000;
  }
  m_chip_select.write(1);
  update_voltage_percent();
}

void BMSMonitoring::update_voltage_percent() {
  if (m_battery_voltage >= 50.40) {
    m_battery_percent_voltage = 100;
  } else if (m_battery_voltage >= 49.80) {
    m_battery_percent_voltage = 95;
  } else if (m_battery_voltage >= 49.32) {
    m_battery_percent_voltage = 90;
  } else if (m_battery_voltage >= 48.96) {
    m_battery_percent_voltage = 85;
  } else if (m_battery_voltage >= 48.24) {
    m_battery_percent_voltage = 80;
  } else if (m_battery_voltage >= 47.76) {
    m_battery_percent_voltage = 75;
  } else if (m_battery_voltage >= 47.40) {
    m_battery_percent_voltage = 65;
  } else if (m_battery_voltage >= 46.44) {
    m_battery_percent_voltage = 60;
  } else if (m_battery_voltage >= 46.20) {
    m_battery_percent_voltage = 55;
  } else if (m_battery_voltage >= 46.08) {
    m_battery_percent_voltage = 50;
  } else if (m_battery_voltage >= 45.84) {
    m_battery_percent_voltage = 45;
  } else if (m_battery_voltage >= 45.60) {
    m_battery_percent_voltage = 40;
  } else if (m_battery_voltage >= 45.48) {
    m_battery_percent_voltage = 35;
  } else if (m_battery_voltage >= 45.24) {
    m_battery_percent_voltage = 30;
  } else if (m_battery_voltage >= 45.00) {
    m_battery_percent_voltage = 25;
  } else if (m_battery_voltage >= 44.76) {
    m_battery_percent_voltage = 20;
  } else if (m_battery_voltage >= 44.52) {
    m_battery_percent_voltage = 15;
  } else if (m_battery_voltage >= 44.28) {
    m_battery_percent_voltage = 10;
  } else if (m_battery_voltage >= 43.32) {
    m_battery_percent_voltage = 5;
  } else {
    m_battery_percent_voltage = 0;
  }
}

void BMSMonitoring::current_monitoring() {
  float current = (m_currentADC.read() * 20) - 33;
  
  current = std::clamp(current, 0.0f, 29.0f);

  current *= 1000;                                           // converts Amps to MilliAmps
  m_coloumb_count += current * 0.001;                        // converts to MilliAmp Seconds
  float battery_capacity_seconds = m_battery_capacity * 3600;  // converts to MilliAmp Seconds
  m_battery_percent_couloumb     = (1 - (m_coloumb_count / battery_capacity_seconds)) * 100;
}

void BMSMonitoring::low_battery_warning() {
  if (m_battery_percent_couloumb <= 15 || m_battery_percent_voltage <= 15) {
    m_buzzer_en.write(1);
  } else {
    m_buzzer_en.write(0);
  }
}

void BMSMonitoring::send_can_data() {
  // TODO: Send data over CAN
  // Things to send over CAN
  // - cell voltages for 12 cells: cell_voltages
  // - percent battery: battery_percent_couloumb and battery_percent_couloumb
  // - current: current
  // setTXSignalValue(HWBRIDGE::CANID msgID, HWBRIDGE::CANSIGNAL signalName, HWBRIDGE::CANSignalValue_t signalValue);
}

void BMSMonitoring::periodic_1s() {
  low_battery_warning();
  send_can_data();
}

void BMSMonitoring::periodic_1ms() {
  current_monitoring();
  cell_monitoring();
}
