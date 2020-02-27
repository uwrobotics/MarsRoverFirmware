#include "mbed.h"
#include "mbed_config.h"
#include "can_config.h"

#include "ScienceConfig.h"
#include "EncoderRelative_Quadrature.h"
#include "EncoderAbsolute_PWM.h"
#include "Servo.h"
#include "CANMsg.h"
#include "CANBuffer.h"

// Motors
Motor indexerMotor(MTR_PWM_1, MTR_DIR_1, false);
Motor diggerLiftMotor(MTR_PWM_2, MTR_DIR_2, false);

// Encoders
EncoderRelative_Quadrature diggerLiftEncoder(ScienceConfig::diggerLiftEncoderConfig);
EncoderAbsolute_PWM indexerEncoder(ScienceConfig::indexerEncoderConfig);

// Servos
Servo coverServo(SRVO_PWM_1, ScienceConfig::coverServoType, ScienceConfig::coverServoRange, ScienceConfig::coverServoMaxPulse, ScienceConfig::coverServoMinPulse);
Servo diggerServo(SRVO_PWM_2, ScienceConfig::diggerServoType, ScienceConfig::diggerServoRange, ScienceConfig::diggerServoMaxPulse, ScienceConfig::diggerServoMinPulse);

// Limit Switches
DigitalIn indexerLimLeft(LIM_SW_1);
DigitalIn indexerLimRight(LIM_SW_2);
DigitalIn diggerLiftLimTop(LIM_SW_3);
DigitalIn diggerLiftLimBot(LIM_SW_4);

// Rotary actuators
ActuatorController indexerActuator(ScienceConfig::indexerActuatorConfig, indexerMotor, indexerEncoder, indexerLimLeft, indexerLimRight);
ActuatorController diggerLiftActuator(ScienceConfig::diggerLiftActuatorConfig, diggerLiftMotor, diggerLiftEncoder, diggerLiftLimBot, diggerLiftLimTop);

// LEDs :D
DigitalOut led1(LED1);
DigitalOut ledR(LED_R);
DigitalOut ledG(LED_G);
DigitalOut ledB(LED_B);


static mbed_error_status_t setMotionData(CANMsg &msg) {
    float motionData;
    msg.getPayload(motionData);

    switch(msg.id) {
        case CANID::SET_INDEXER_POS:
            return indexerActuator.setMotionData(motionData);
        case CANID::SET_LIFT_POS:
            return diggerLiftActuator.setMotionData(motionData);
        case CANID::SET_COVER_POS:
            return coverServo.set_position(motionData);
        case CANID::SET_DIGGER_POS:
            return diggerServo.set_position(motionData);
        default:
            return MBED_ERROR_INVALID_ARGUMENT;
    }
}

// Interface and recieve buffer
CAN can1(CAN1_RX, CAN1_TX, ROVER_CANBUS_FREQUENCY);

// CAN Threads
Thread rxCANProcessorThread;
Thread txCANProcessorThread;

static CANMsg::CANMsgHandlerMap canHandleMap = {
    {CANID::SET_INDEXER_POS, setMotionData},
    {CANID::SET_LIFT_POS,    setMotionData},
    {CANID::SET_COVER_POS,   setMotionData},
    {CANID::SET_DIGGER_POS,  setMotionData}
};

// Process incoming CAN messages
void rxCANProcessor() {
    CANMsg rxMsg;

    while(true) {
        if(can1.read(rxMsg)) {
            if(canHandleMap.count(rxMsg.id) > 0) {
                canHandleMap[rxMsg.id](rxMsg);
            } else {
                // ruh roh
            }
        }
    }
}

// Send outgoing CAN messages
void txCANProcessor(){
    const int txPeriod_millisec = 500;
    CANMsg txMsg;

    while(true) {
        txMsg.id = SEND_INDEXER_POS;
        txMsg.setPayload(indexerActuator.getAngle_Degrees());
        can1.write(txMsg);
        ThisThread::sleep_for(txPeriod_millisec);

        txMsg.id = SEND_LIFT_POS;
        txMsg.setPayload(diggerLiftActuator.getAngle_Degrees());
        can1.write(txMsg);
        ThisThread::sleep_for(txPeriod_millisec);

        txMsg.id = SEND_COVER_POS;
        txMsg.setPayload(coverServo.read());
        can1.write(txMsg);
        ThisThread::sleep_for(txPeriod_millisec);
        
        txMsg.id = SEND_DIGGER_POS;
        txMsg.setPayload(diggerServo.read());
        can1.write(txMsg);
        ThisThread::sleep_for(txPeriod_millisec);
    }
}


int main() {
    printf("\r\n\r\n");
    printf("SCIENCE APP STARTED!\r\n");
    printf("====================\r\n");

    rxCANProcessorThread.start(rxCANProcessor);
    txCANProcessorThread.start(txCANProcessor);

    while(true) {
        indexerActuator.update();
        diggerLiftActuator.update();

        ThisThread::sleep_for(2);
    }
}
