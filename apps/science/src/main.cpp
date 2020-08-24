#include "ActuatorController.h"
#include "CANBuffer.h"
#include "CANMsg.h"
#include "Encoder.h"
#include "EncoderAbsolute_PWM.h"
#include "EncoderRelative_Quadrature.h"
#include "MoistureSensor.h"
#include "ScienceConfig.h"
#include "Servo.h"
#include "can_config.h"
#include "mbed.h"
#include "mbed_config.h"

// motors
Motor elevatorMotor(MTR_PWM_2, MTR_DIR_2, false);
Motor indexerMotor(MTR_PWM_1, MTR_DIR_1, false);

// Servo
Servo coverServo(SRVO_PWM_1, ScienceConfig::coverServoType, ScienceConfig::coverServoRange,
                 ScienceConfig::coverServoMaxPulse, ScienceConfig::coverServoMinPulse);
Servo diggerServo(SRVO_PWM_2, ScienceConfig::diggerServoType, ScienceConfig::diggerServoRange,
                  ScienceConfig::diggerServoMaxPulse, ScienceConfig::diggerServoMinPulse);

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
CAN can(CAN1_RX, CAN1_TX, ROVER_CANBUS_FREQUENCY);

// k interval
const int k_interval_ms = 500;

// static objects
static mbed_error_status_t setMotionData(CANMsg &msg) {
  float motionData;
  msg.getPayload(motionData);

  switch (msg.id) {
    case CANID::SET_INDEXER_POS:
      return indexerActuator.setMotionData(motionData);
    case CANID::SET_ELEVATOR_POS:
      return elevatorActuator.setMotionData(motionData);
    case CANID::SET_COVER_POS:
      return coverServo.set_position(motionData);
    case CANID::SET_DIGGER_POS:
      return diggerServo.set_position(motionData);
    case CANID::SET_MOISTURE_SENSOR:
      return moistureSensor.Is_Initialized();
    default:
      return MBED_ERROR_INVALID_ARGUMENT;
  }
}

static CANMsg::CANMsgHandlerMap canHandleMap = {{CANID::SET_INDEXER_POS, setMotionData},
                                                {CANID::SET_ELEVATOR_POS, setMotionData},
                                                {CANID::SET_COVER_POS, setMotionData},
                                                {CANID::SET_DIGGER_POS, setMotionData},
                                                {CANID::SET_MOISTURE_SENSOR, setMotionData}};

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
    txMsg.id = SEND_INDEXER_POS;
    txMsg.setPayload(indexerActuator.getAngle_Degrees());
    can.write(txMsg);
    ThisThread::sleep_for(txPeriod_millisec);

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
