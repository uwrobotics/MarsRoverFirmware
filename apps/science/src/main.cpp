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

    // YOUNES COMMENT DELETE ONCE READ: CANIDS WERE UPDATE IN THE HW BRIDGE
    /* THERES 4 CANIDS THAT U CAN RECEIVE. THEY ASK YOU TO MAKE CERTAIN MOTORS MOVE A CERTAIN WAY*/
    // THERES 6 CANIDS U CAN SEND. IMPLEMENT ALL THE REPORTS EXCEPT FOR REPORT DIGGER AND REPORT COVER

    /* SOME QUESTIONS YOU NEED TO THINK ABT AND ANSWER: 
    HOW DO WE TRANSLATE AN INDEX TO A CMD WE CAN GIVE THE MOTOR (FOR SET INDEX FUNCITONALITY) NEED TO UNDERSTAND GENEVA DRIVE MECHANISM

    */


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
    // we need indexes for indexer pos, elevator pos, and moisture sensor
    case HWBRIDGE::CANID::SET_INDEXER_POS:  // resolve conflict in the future
      return indexerActuator.setMotionData(motionData);
    case HWBRIDGE::CANID::SET_ELEVATOR_POS:  // resolve conflict in the future
      return elevatorActuator.setMotionData(motionData);
    case HWBRIDGE::CANID::SET_COVER_INDEX:  // this one works
      return coverServo.setPosition(motionData);
    case HWBRIDGE::CANID::SET_DIGGER_LIFT_HEIGHT:  // this one works
      return diggerServo.setPosition(motionData);
    case HWBRIDGE::CANID::SET_MOISTURE_SENSOR:  // this one will be added later
      return moistureSensor.Is_Initialized();
    default:
      return MBED_ERROR_INVALID_ARGUMENT;
  }
}
// it seems like a lot of these are not listed in the hw_bridge namespace, perhaps they are all under different names
// now?
static CANMsg::CANMsgHandlerMap canHandleMap = {{HWBRIDGE::CANID::SET_INDEXER_POS, setMotionData},
                                                {HWBRIDGE::CANID::SET_ELEVATOR_POS, setMotionData},
                                                {HWBRIDGE::CANID::SET_COVER_INDEX, setMotionData},
                                                {HWBRIDGE::CANID::SET_DIGGER_LIFT_HEIGHT, setMotionData},
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
  constexpr std::chrono::milliseconds txPeriod = 500ms;
  CANMsg txMsg;

  while (true) {
    txMsg.id = SEND_INDEXER_POS;
    txMsg.setPayload(indexerActuator.getAngle_Degrees());
    can.write(txMsg);
    ThisThread::sleep_for(txPeriod);

    txMsg.id = SEND_ELEVATOR_POS;
    txMsg.setPayload(elevatorActuator.getAngle_Degrees());
    can.write(txMsg);
    ThisThread::sleep_for(txPeriod);

    txMsg.id = HWBRIDGE::CANID::REPORT_COVER_POS;
    txMsg.setPayload(coverServo.read());
    can.write(txMsg);
    ThisThread::sleep_for(txPeriod);

    txMsg.id =
        HWBRIDGE::CANID::REPORT_DIGGER_LIFT_POS;  // i think this is a duplicate as the block of code below is the same
    txMsg.setPayload(diggerServo.read());
    can.write(txMsg);
    ThisThread::sleep_for(txPeriod);

    txMsg.id = HWBRIDGE::CANID::REPORT_DIGGER_LIFT_POS;
    txMsg.setPayload(diggerServo.read());
    can.write(txMsg);
    ThisThread::sleep_for(txPeriod);

    // Read moisture returns an unsigned number so it needs to be cast to an int to be handled
    txMsg.id = HWBRIDGE::CANID::REPORT_MOISTURE_DATA;
    txMsg.setPayload((int)moistureSensor.Read_Moisture());
    can.write(txMsg);
    ThisThread::sleep_for(txPeriod);

    txMsg.id = HWBRIDGE::CANID::REPORT_TEMPERATURE_DATA;
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
