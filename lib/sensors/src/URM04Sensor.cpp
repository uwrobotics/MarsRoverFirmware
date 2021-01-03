#include "URM04Sensor.h"

// instantiate pin connected to the URM04 URM04Sensor
URM04Sensor::URM04Sensor::URM04Sensor(PinName trig_pin, PinName _RX, PinName _TX)
    : m_trigPin(trig_pin), startAddr(START_ADDRESS), RX(_RX), TX(_TX), serial(TX, RX, BAUD_RATE) {
  // write low to pin to start instructions
  m_trigPin.write(LOW);

  // instantiate command buffer with all zeros

  /***
   * for(int i{0}; i<10; i++){
   *   cmdst[i] = 0;
   * }
   *
   ***/

  memset(&cmdst[0], 0, sizeof(cmdst));
}

void URM04Sensor::URM04Sensor::trigger_sensor() {
  /************************************ INSTANTIATE COMMANDS TO BE SENT OVER SERIAL
   * *****************************************/

  // check sum represents the final bit in command buffer - made by adding all previous bits in command buffer
  uint8_t checkSum;
  // buffer header
  cmdst[0] = 0x055;
  cmdst[1] = 0xAA;
  // device address
  cmdst[2] = startAddr;
  // length
  cmdst[3] = 0x00;
  // the command itself
  cmdst[4] = 0x01;
  // checksum bit
  checkSum = cmdst[0] + cmdst[1] + cmdst[2] + cmdst[3] + cmdst[4];
  // instantiate the last element in the command buffer with checksum value
  cmdst[5] = checkSum;

  /******** SEND COMMANDS OVER SERIAL *********************/
  int num_bytes;
  // returns the number of bytes if successful write
  num_bytes = serial.write(&cmdst[0], sizeof(cmdst));

  // else if numb_btyes < 0 serial failed or if number of bytes written != 6 (because 6 bytes sent over serial)
  if (num_bytes < 0 || num_bytes != 6) {
    success = false;
  }

  // flush the buffer from serial
  serial.sync();

  // wait for at least 30 ms after calling trigger function
  ThisThread::sleep_for(std::chrono::milliseconds(35));

  // reset command buffer - fill whole array with zeros
  memset(&cmdst[0], 0, sizeof(cmdst));
}

float URM04Sensor::URM04Sensor::read_distance() {
  /************ INSTANTIATE COMMANDS TO BE SENT OVER SERIAL************/
  // check sum represents the final bit in command buffer - made by adding all previous bits in command buffer
  uint8_t checkSum;
  // buffer header
  cmdst[0] = 0x55;
  cmdst[1] = 0xAA;
  // device address
  cmdst[2] = startAddr;
  // command length
  cmdst[3] = 0x00;
  // the command itself
  cmdst[4] = 0x02;
  // checksum bit
  checkSum = cmdst[0] + cmdst[1] + cmdst[2] + cmdst[3] + cmdst[4];
  // instantiate the last element in the command buffer with checksum value
  cmdst[5] = checkSum;

  /************** SEND COMMANDS OVER SERIAL***********/
  int w_num_bytes;
  // returns the number of bytes if successful read
  w_num_bytes = serial.write(&cmdst[0], sizeof(cmdst));

  // else if numb_btyes < 0 serial failed or if number of bytes read != 6 (because 6 bytes sent over serial)
  if (w_num_bytes < 0 || w_num_bytes != 6) {
    success = false;
    // return -1 indicating read command has failed
    return -1;
  }

  // flush buffer from serial
  serial.sync();

  // reset command buffer - fill whole array with zeros
  memset(&cmdst[0], 0, sizeof(cmdst));
  /******* READ RETURN VALUE FROM SERIAL**************/
  int r_num_bytes;
  // returns the number of bytes if successful read
  r_num_bytes = serial.read(&cmdst[0], sizeof(cmdst));

  // else if numb_btyes < 0 serial failed or if number of bytes read != 8 (because 8 bytes sent over serial)
  if (r_num_bytes < 0 || r_num_bytes != 8) {
    success = false;
    return -1;
  }
  /******** PARSE THROUGH THE DATA READ FROM SERIAL*********/
  else {
    // create the checksum
    checkSum = cmdst[0] + cmdst[1] + cmdst[2] + cmdst[3] + cmdst[4] + cmdst[5] + cmdst[6];

    // check if the checksum is incorrect
    if (checkSum != cmdst[7]) {
      success = false;
      // return -1 if checksum failed
      return -1;
    } else {
      // get distance from sensor
      m_distance = (float)(cmdst[5] << 8) + (float)cmdst[6];
      // --------------------- or --------------------------
      // m_distance = (float)(cmdst[5]*256) + (float)cmdst[6];
      return m_distance;
    }
  }
}