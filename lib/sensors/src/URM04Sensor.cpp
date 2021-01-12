#include "URM04Sensor.h"

// instantiate pin connected to the URM04 URM04Sensor
URM04Sensor::URM04Sensor::URM04Sensor(PinName trig_pin, PinName _RX, PinName _TX, uint8_t default_address)
    : m_trigPin(trig_pin), m_startAddr(default_address), m_serial(_TX, _RX, BAUD_RATE) {
}

bool URM04Sensor::URM04Sensor::trigger_sensor() {
  // turn on transmitting mode for RS485
  m_trigPin.write(HIGH);
  ThisThread::sleep_for(1ms);
  /************ INSTANTIATE COMMANDS TO BE SENT OVER SERIAL*********/

  // check sum represents the final bit in command buffer - made by adding all previous bits in command buffer
  uint8_t checkSum;
  // buffer header
  m_cmdst[0] = 0x55;
  m_cmdst[1] = 0xAA;
  // device address
  m_cmdst[2] = m_startAddr;
  // length
  m_cmdst[3] = 0x00;
  // the command itself
  m_cmdst[4] = 0x01;
  // checksum bit
  checkSum = m_cmdst[0] + m_cmdst[1] + m_cmdst[2] + m_cmdst[3] + m_cmdst[4];
  // instantiate the last element in the command buffer with checksum value
  m_cmdst[5] = checkSum;

  /******** SEND COMMANDS OVER SERIAL *********************/
  // returns the number of bytes if successful write
  int num_bytes = m_serial.write(&m_cmdst[0], 6);

  // else if numb_btyes < 0 serial failed or if number of bytes written != 6 (because 6 bytes sent over serial)
  if (num_bytes != 6) {
    // error occured in trigger step
    return false;
  } else {
    // no error occured in trigger step
    // flush the buffer from serial
    m_serial.sync();
    // wait for at least 30 ms after calling trigger function
    ThisThread::sleep_for(35ms);
    return true;
  }
}

// pass by reference a variable to hold the distance value - will give MAX_FLOAT if out of range
bool URM04Sensor::URM04Sensor::read_distance(float& distance) {
  /****************** TRIGGER SENSOR BEFORE READING DISTANCE ********************/
  // if trigger fails set distance to -1 and return false
  if (trigger_sensor() == false) {
    distance = -1;
    return false;
  }

  /************ INSTANTIATE COMMANDS TO BE SENT OVER SERIAL************/
  // check sum represents the final bit in command buffer - made by adding all previous bits in command buffer
  uint8_t checkSum;
  // buffer header
  m_cmdst[0] = 0x55;
  m_cmdst[1] = 0xAA;
  // device address
  m_cmdst[2] = m_startAddr;
  // command length
  m_cmdst[3] = 0x00;
  // the command itself
  m_cmdst[4] = 0x02;
  // checksum bit
  checkSum = m_cmdst[0] + m_cmdst[1] + m_cmdst[2] + m_cmdst[3] + m_cmdst[4];
  // instantiate the last element in the command buffer with checksum value
  m_cmdst[5] = checkSum;

  /************** SEND COMMANDS OVER SERIAL***********/
  // returns the number of bytes if successful read
  int w_num_bytes = m_serial.write(&m_cmdst[0], 6);

  // else if numb_btyes < 0 serial failed or if number of bytes read != 6 (because 6 bytes sent over serial)
  if (w_num_bytes != 6) {
    // return false indicating read command has failed
    distance = -1;
    return false;
  }

  // flush buffer from serial
  m_serial.sync();

  /******* READ RETURN VALUE FROM SERIAL**************/

  // turn on reading mode for RS485
  m_trigPin.write(LOW);
  ThisThread::sleep_for(1ms);

  // returns the number of bytes if successful read
  int r_num_bytes = m_serial.read(&m_cmdst[0], 8);

  // else if numb_btyes < 0 serial failed or if number of bytes read != 8 (because 8 bytes sent over serial)
  if (r_num_bytes != 8) {
    distance = -1;
    return false;
  }
  /******** PARSE THROUGH THE DATA READ FROM SERIAL*********/
  else {

    // check if the checksum is incorrect
    if (m_cmdst[7] != m_cmdst[0] + m_cmdst[1] + m_cmdst[2] + m_cmdst[3] + m_cmdst[4] + m_cmdst[5] + m_cmdst[6]) {
      // return false if checksum failed
      distance = -1;
      return false;
    } else {
      // get distance from sensor
      distance = (float)(m_cmdst[5] << 8) + (float)m_cmdst[6];
      // --------------------- or --------------------------
      // distance = (float)(m_cmdst[5]*256) + (float)m_cmdst[6];

      // check if distance is out of range - if so return maximum float value
      if (m_cmdst[5] == 0xFF && m_cmdst[6] == 0xFF) {
        distance = MAX_FLOAT;
      }
      return true;
    }
  }
  // flush serial
  m_serial.sync();
}

bool URM04Sensor::URM04Sensor::set_address(uint8_t _address) {
  // turn on RS485 transmit mode
  m_trigPin.write(HIGH);
  ThisThread::sleep_for(1ms);

  /************ INSTANTIATE COMMANDS TO BE SENT OVER SERIAL************/
  // check sum represents the final bit in command buffer - made by adding all previous bits in command buffer
  uint8_t checkSum;
  // buffer header
  m_cmdst[0] = 0x55;
  m_cmdst[1] = 0xAA;
  // device address
  m_cmdst[2] = 0xAB;
  // command length
  m_cmdst[3] = 0x01;
  // the command itself
  m_cmdst[4] = 0x55;
  // set new Address
  m_cmdst[5] = _address;
  // compute checksum bit
  checkSum = m_cmdst[0] + m_cmdst[1] + m_cmdst[2] + m_cmdst[3] + m_cmdst[4] + m_cmdst[5];
  // instantiate the last element in the command buffer with checksum value
  m_cmdst[6] = checkSum;

  /************** SEND COMMANDS OVER SERIAL***********/
  // send command over serial - write
  int w_num_bytes = m_serial.write(&m_cmdst[0], 7);
  // check if command was successfully written to the serial
  if (w_num_bytes != 7) {
    return false;
  }
  // flush the serial
  m_serial.sync();

  /************* PARSE THROUGH DATA RECIEVED***********/

  // turn on RS485 reading mode
  m_trigPin.write(LOW);
  ThisThread::sleep_for(1ms);

  // read return value from serial
  int r_num_bytes = m_serial.read(&m_cmdst[0], 7);
  // check if read is successful
  if (r_num_bytes != 7) {
    return false;
  }
  // check if the checksum is incorrect
  if (m_cmdst[6] != m_cmdst[0] + m_cmdst[1] + m_cmdst[2] + m_cmdst[3] + m_cmdst[4] + m_cmdst[5]) {
    return false;
  } else {
    // check if the command went through successfully by checking if the
    // 6th element of the return buffer array == 0x01
    if (m_cmdst[5] == 0x01) {
      // new address is set
      m_startAddr = _address;

      // clean up
      m_serial.sync();

      return true;
    }
    return false;
  }
}
