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

// Servo
LimServo coverServo(SRVO_PWM_1, ScienceConfig::coverServoRange, ScienceConfig::coverServoMaxPulse,
                    ScienceConfig::coverServoMinPulse);

LimServo diggerServo(SRVO_PWM_2, ScienceConfig::diggerServoRange, ScienceConfig::diggerServoMaxPulse,
                     ScienceConfig::diggerServoMinPulse);

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

// LED used for debugging
DigitalOut led1(LED1);

// Declaring CANBus object
CANBus can(CAN1_RX, CAN1_TX, HWBRIDGE::ROVERCONFIG::ROVER_CANBUS_FREQUENCY);

// static objects
static mbed_error_status_t setMotionData(CANMsg &msg) {
  float motionData;
  msg.getPayload(motionData);

  switch (msg.getID()) {
    case HWBRIDGE::CANID::SET_GENEVA_INDEX:
      return indexerActuator.setMotionData(motionData);
    case HWBRIDGE::CANID::SET_SCOOPER_ANGLE:  // not sure what this case does will contact younes soon to discuss
      return elevatorActuator.setMotionData(motionData);  // with him (perhaps this is elevatorActuator??)
    case HWBRIDGE::CANID::SET_COVER_ANGLE:
      return coverServo.setPosition(motionData);
    case HWBRIDGE::CANID::SET_ELEVATOR_HEIGHT:
      return diggerServo.setPosition(motionData);
    default:
      return MBED_ERROR_INVALID_ARGUMENT;
  }
}

static CANMsg::CANMsgHandlerMap canHandleMap = {{HWBRIDGE::CANID::SET_GENEVA_INDEX, setMotionData},
                                                {HWBRIDGE::CANID::SET_SCOOPER_ANGLE, setMotionData},
                                                {HWBRIDGE::CANID::SET_COVER_ANGLE, setMotionData},
                                                {HWBRIDGE::CANID::SET_ELEVATOR_HEIGHT, setMotionData}};

// CAN Threads
Thread rxCANProcessorThread;
Thread txCANProcessorThread;

// recieving CAN messages
void rxCANProcessor() {
  CANMsg rxMsg;
  while (true) {
    if (can.read(rxMsg)) {
      canHandleMap.at(rxMsg.getID())(rxMsg);
    }
  }
}

// Send outgoing CAN messages
void txCANProcessor() {
  constexpr std::chrono::milliseconds txPeriod = 500ms;
  CANMsg txMsg;

  while (true) {
    txMsg.setID(HWBRIDGE::CANID::REPORT_GENEVA_INDEX);
    txMsg.setPayload(indexerActuator.getAngle_Degrees());
    can.write(txMsg);
    ThisThread::sleep_for(txPeriod);

    txMsg.setID(HWBRIDGE::CANID::REPORT_SCOOPER_ANGLE);
    txMsg.setPayload(elevatorActuator.getAngle_Degrees());
    can.write(txMsg);
    ThisThread::sleep_for(txPeriod);

    txMsg.setID(HWBRIDGE::CANID::REPORT_COVER_ANGLE);
    txMsg.setPayload(coverServo.read());
    can.write(txMsg);
    ThisThread::sleep_for(txPeriod);

    txMsg.setID(HWBRIDGE::CANID::REPORT_ELEVATOR_HEIGHT);
    txMsg.setPayload(diggerServo.read());
    can.write(txMsg);
    ThisThread::sleep_for(txPeriod);

    // Read moisture returns an unsigned number so it needs to be cast to an int to be handled
    txMsg.setID(HWBRIDGE::CANID::REPORT_MOISTURE_DATA);
    txMsg.setPayload(static_cast<int>(moistureSensor.Read_Moisture()));
    can.write(txMsg);
    ThisThread::sleep_for(txPeriod);

    txMsg.setID(HWBRIDGE::CANID::REPORT_TEMPERATURE_DATA);
    txMsg.setPayload(moistureSensor.Read_Temperature());
    can.write(txMsg);
    ThisThread::sleep_for(txPeriod);
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

    ThisThread::sleep_for(2000ms);
  }
}
