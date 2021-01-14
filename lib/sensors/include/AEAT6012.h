#pragma once

/*
 * Broadcom's AEAT-6012 is an absolute magnetic encoder with 12 bit resolution and SPI interface
 * Datasheet: https://docs.broadcom.com/doc/AV02-0188EN
 *
 * Currently a WIP
 *
 * TODO:
 * - Make driver async
 * - Incorporate under Encoder base class
 */

class AEAT6012 {
 public:
  AEAT6012(PinName cs, PinName spi_mosi, PinName spi_clk, int frequency_hz = DEFAULT_FREQUENCY_HZ);

  // Returns absolute position in degrees
  float read_position(void);

 private:
  static constexpr int DEFAULT_FREQUENCY_HZ = 1000000;  // Max frequency given by datasheet. TODO: Need to test this
  uint8_t dummy_buffer[2]                   = {0x00, 0x00};
  uint8_t read_buffer[2]                    = {0x00, 0x00};
  DigitalOut m_cs;
  SPI m_spi;

  static float raw_data_to_degrees(uint16_t raw_data);  // Converts encoding reading to degrees
};
