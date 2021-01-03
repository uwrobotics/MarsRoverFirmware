#include "URM04Sensor.h"

// instantiate pin connected to the URM04 URM04Sensor
URM04Sensor::URM04Sensor::URM04Sensor(PinName trig_pin, PinName _RX, PinName _TX)
    : m_trigPin(trig_pin), startAddr(START_ADDRESS), readingStep(LOW), RX(_RX), TX(_TX), read_success(false), serial(TX, RX, BAUD_RATE) {
  // start timer
  clock.start();
  // get start time
  managerTimer =
    std::chrono::duration_cast<std::chrono::milliseconds>(chrono::duration_cast<chrono::duration<uint64_t>>(clock.elapsed_time())).count();
  // write low to pin to start instructions
  m_trigPin.write(LOW);

  // Initialize urm04 command recieving address
  for (int i{0}; i < urmAccount; i++) {
    urmID[i]   = startAddr + 1;
    urmData[i] = 0;
  }

  // initialise commands - set zero everywhere
  for (int i{0}; i < 10; i++) {
    cmdst[i] = 0;
  }

  // initialize urm04 protocol
  cmdst[0] = 0x55;
  cmdst[1] = 0xaa;

  // set nucleo_board baud rate

  // set up serial communication uart bufferserial with baud rate
  // serial communication - D1/TX , D0/RX,
}
// destructor
URM04Sensor::URM04Sensor::~URM04Sensor() {
  // kill the timer
  clock.stop();
}
// trigger the measurements
void URM04Sensor::URM04Sensor::urmTrigger(int id) {
  // fill the command buffer with trigger commands
  cmdst[2] = id;
  cmdst[3] = 0x00;
  cmdst[4] = 0x01;
  // send command over serial
  transmitCommands();
}
// reads the distance from URM04
void URM04Sensor::URM04Sensor::urmReader(int id) {
  // fill command buffer with read commands
  cmdst[2] = id;
  cmdst[3] = 0x00;
  cmdst[4] = 0x02;
  // send command over serial
  transmitCommands();
}
// transmit commands
void URM04Sensor::URM04Sensor::transmitCommands() {
  cmdst[5] = cmdst[0] + cmdst[2] + cmdst[3] + cmdst[4];
  // delay for 1 second using thread  1000ms = 1sec
  ThisThread::sleep_for(std::chrono::milliseconds(1000));
  // send command over serial connection with BufferedSerial write and put into cmdst
  serial.write(&cmdst[0], sizeof(cmdst));
  // delay for 2 seconds 2000ms = 2sec
  ThisThread::sleep_for(std::chrono::milliseconds(2000));
}

// this function actually gets the data
void URM04Sensor::URM04Sensor::analyzeUrmData(uint8_t cmd[]) {
  uint8_t checkSum = 0;
  // add each byte in the command array to create checksum
  for (int i{0}; i < 7; i++) {
    checkSum += cmd[i];
  }
  // check is checksum is correct and that the commands are correct as well
  if (checkSum == cmd[7] && cmd[3] == 2 && cmd[4] == 2) {
    uint8_t id  = cmd[2] - startAddr;
    urmData[id] = cmd[5] * 256 + cmd[6];
  }
  // else an error occured during transmission
  else if (cmd[3] == 2 && cmd[4] == 2) {
    read_success = false;
  }
}

// decodes URM04 data
void URM04Sensor::URM04Sensor::decodeURM04() {
  // make sure serial is still running
  if (serial.readable()) {
    // timer
    //int timerPoint = clock.elapsed_time(); //cast this 
    // counter
    int RetryCounter{0};
    // array to hold commands
    uint8_t cmdrd[10];
    // fill the array
    for (int i{0}; i < 10; i++) {cmdrd[i] = 0;}
    // start index & indices
    int indices{0};
    int start_index{0};
    // flag to check if the indices in the buffer are valid
    bool flag{true};
    // variable to check if the whole buffer is valid
    bool valid{false};
    // header bit
    uint8_t headerNo{0};

    while (RetryCounter < MAX_TRIES && flag) {
      // ensure serial is still running
      if (serial.readable()) {
        // read from the serial port with BufferedSerial read and put  data into cmdrd
        indices = serial.read(cmdrd, sizeof(cmdrd));
        // did not read from serial port properly
        if (indices == -1) {
          flag = false;
          // flush buffer
          serial.sync();
          // read failed
          read_success = false;
          break;  // exit loop
        }
        // confirm the header address is correct
        if (cmdrd[start_index] == 0xAA) {
          headerNo = 1;
          valid    = true;
        }
        // confirm every bit is within the range of the buffer
        if (valid && start_index == headerNo + 6) {
          flag = false;
          break;  // exit loop
        }

        start_index++;
        RetryCounter = 0;

      } else {
        RetryCounter++;
        wait_us(15);
      }
    }
    // since all the data has proven to be valid send the data to be analyzed
    if (valid) {
      read_success = true;
      analyzeUrmData(cmdrd);
    }
  }
}
// run the urm04
void URM04Sensor::URM04Sensor::runUrm04() {
  static uint64_t timer{0};
  static int num{0};

  if ((uint64_t)clock.read_ms() - timer > managerTimer) {
    // write high to trip pin to turn on RS485 Transmitting mode
    m_trigPin.write(HIGH);

    // state machine to read measurements from the URM04Sensor
    switch (readingStep) {
      case 0:
        urmTrigger(urmID[num]);
        managerTimer = 40;  // set interval for timed measurement after triggering measurements
        break;
      case 1:
        urmReader(urmID[num]);
        managerTimer = 0;  // set interval for measurements after reading distance command
        break;
      case 2:
        m_trigPin.write(LOW);  // turn on reading mode for RS485
        managerTimer = 10;
        break;
      default:
        readingStep = 0;
        break;
    }

    if (readingStep < 2)
      readingStep++;
    else
      readingStep = 0;

    timer = 
          std::chrono::duration_cast<std::chrono::milliseconds>(chrono::duration_cast<chrono::duration<uint64_t>>(clock.elapsed_time())).count();

  }
}
// this function runs the URM04Sensor
bool URM04Sensor::URM04Sensor::read() {
  runUrm04();
  decodeURM04();
  return read_success;
}
// this function prints the data from the URM04Sensor
void URM04Sensor::URM04Sensor::print_distance() {
  for (int i{0}; i < urmAccount; i++) {
    printf("%lu", urmData[i]); //read 
  }
  wait_us(15);
}
