#include "ActuatorController.h"
#include "CANBuffer.h"
#include "CANMsg.h"
#include "Encoder.h"
#include "EncoderAbsolute_PWM.h"
#include "EncoderRelative_Quadrature.h"
#include "LimServo.h"
#include "MoistureSensor.h"
#include "ScienceConfig.h"
#include "hw_bridge.h"
#include "mbed.h"

// motors
Motor elevatorMotor(MTR_PWM_2, MTR_DIR_2, false);
Motor indexerMotor(MTR_PWM_1, MTR_DIR_1, false);

/*
// Servo
LimServo coverServo(SRVO_PWM_1, ScienceConfig::coverServoRange, ScienceConfig::coverServoMaxPulse,
                    ScienceConfig::coverServoMinPulse);  // these two constructors are not working
// we are recieving an error saying that there is no such constructor that exists
LimServo diggerServo(SRVO_PWM_2, ScienceConfig::diggerServoRange, ScienceConfig::diggerServoMaxPulse,
                     ScienceConfig::diggerServoMinPulse);
*/

// encoders
EncoderAbsolute_PWM elevatorEncoder(ScienceConfig::elevatorEncoderConfig);
EncoderRelative_Quadrature indexerEncoder(ScienceConfig::centrifugeEncoderConfig);

// limit switches
DigitalIn elevatorLimTop(LIM_SW_3);
DigitalIn elevatorLimBottom(LIM_SW_4);
DigitalIn indexerLimFront(LIM_SW_1);
DigitalIn indexerLimBack(LIM_SW_2);

// Actuators
ActuatorController elevatorActuator(ScienceConfig::diggerLiftActuatorConfig, elevatorMotor, elevatorEncoder,
                                    elevatorLimBottom, elevatorLimTop);
ActuatorController indexerActuator(ScienceConfig::indexerActuatorConfig, indexerMotor, indexerEncoder, indexerLimFront,
                                   indexerLimBack);

// I2C
MoistureSensor moistureSensor = MoistureSensor(TEMP_MOIST_I2C_SDA, TEMP_MOIST_I2C_SCL);

// LEDs
DigitalOut led1(LED1);
DigitalOut ledR(LED_R);
DigitalOut ledG(LED_G);
DigitalOut ledB(LED_B);

// Declaring can object
CAN can(CAN1_RX, CAN1_TX, HWBRIDGE::ROVERCONFIG::ROVER_CANBUS_FREQUENCY);

// k interval
const int k_interval_ms = 500;

// static objects
static mbed_error_status_t setMotionData(CANMsg &msg) {
  float motionData;
  msg.getPayload(motionData);

  switch (msg.id) {
    case HWBRIDGE::CANID::SET_INDEXER_POS:  // not declared in hw_bridge (included in can_config.h)
      return indexerActuator.setMotionData(motionData);
    case HWBRIDGE::CANID::SET_ELEVATOR_POS:  // not declared in hw_bridge (included in can_config.h)
      return elevatorActuator.setMotionData(motionData);
    case HWBRIDGE::CANID::SET_COVER_POS:  // not declared in hw_bridge (included in can_config.h)
      return coverServo.set_position(motionData);
    case HWBRIDGE::CANID::SET_DIGGER_POS:  // not declared in hw_bridge (included in can_config.h)
      return diggerServo.set_position(motionData);
    case HWBRIDGE::CANID::SET_MOISTURE_SENSOR:  // not declared in hw_bridge (included in can_config.h)
      return moistureSensor.Is_Initialized();
    default:
      return MBED_ERROR_INVALID_ARGUMENT;
  }
}
// it seems like a lot of these are not listed in the hw_bridge namespace, perhaps they are all under different names
// now?
static CANMsg::CANMsgHandlerMap canHandleMap = {{HWBRIDGE::CANID::SET_INDEXER_POS, setMotionData},
                                                {HWBRIDGE::CANID::SET_ELEVATOR_POS, setMotionData},
                                                {HWBRIDGE::CANID::SET_COVER_POS, setMotionData},
                                                {HWBRIDGE::CANID::SET_DIGGER_POS, setMotionData},
                                                {HWBRIDGE::CANID::SET_MOISTURE_SENSOR, setMotionData}};

// CAN Threads
Thread rxCANProcessorThread;
Thread txCANProcessorThread;

// recieving CAN messages
void rxCANProcessor() {
  CANMsg rxMsg;

  while (true) {
    if (can.read(rxMsg)) {
      if (canHandleMap.count(rxMsg.id) > 0) {
        canHandleMap[rxMsg.id](rxMsg);
      } else {
        // ruh roh
      }
    }
  }
}

// Send outgoing CAN messages
void txCANProcessor() {
  const int txPeriod_millisec = 500;
  CANMsg txMsg;

  while (true) {
    txMsg.id = SEND_INDEXER_POS;  // again this is in the can_config.h file
    txMsg.setPayload(indexerActuator.getAngle_Degrees());
    can.write(txMsg);
    ThisThread::sleep_for(txPeriod_millisec);  // error says we should provide an integer number of seconds to sleep for

    txMsg.id = SEND_ELEVATOR_POS;
    txMsg.setPayload(elevatorActuator.getAngle_Degrees());
    can.write(txMsg);
    ThisThread::sleep_for(txPeriod_millisec);

    txMsg.id = SEND_COVER_POS;
    txMsg.setPayload(coverServo.read());
    can.write(txMsg);
    ThisThread::sleep_for(txPeriod_millisec);

    txMsg.id = SEND_DIGGER_POS;

    txMsg.setPayload(diggerServo.read());
    can.write(txMsg);
    ThisThread::sleep_for(txPeriod_millisec);

    txMsg.id = SEND_DIGGER_POS;
    txMsg.setPayload(diggerServo.read());
    can.write(txMsg);
    ThisThread::sleep_for(txPeriod_millisec);

    // Read moisture returns an unsigned number so it needs to be cast to an int to be handled
    txMsg.id = SEND_MOISTURE;
    txMsg.setPayload((int)moistureSensor.Read_Moisture());
    can.write(txMsg);
    ThisThread::sleep_for(txPeriod_millisec);

    txMsg.id = SEND_TEMPERATURE;
    txMsg.setPayload(moistureSensor.Read_Temperature());
    can.write(txMsg);
    ThisThread::sleep_for(txPeriod_millisec);
  }
}

int main() {
  printf("\r\n\r\n");
  printf("SCIENCE APP STARTED!\r\n");
  printf("====================\r\n");

  rxCANProcessorThread.start(rxCANProcessor);
  txCANProcessorThread.start(txCANProcessor);

  while (true) {
    indexerActuator.update();
    elevatorActuator.update();

    ThisThread::sleep_for(2);
  }
}
