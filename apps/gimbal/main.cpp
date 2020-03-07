#include "../targets/gimbal/pinNames.h"

#include "mbed.h"
#include "mbed_error.h"
#include "mbed_config.h"
#include "can_config.h"
#include "rtos.h"

#include "GimbalConfig.h"
#include "Encoder.h"
#include "EncoderAbsolute_PWM.h"
#include "ActuatorController.h"
#include "CANMsg.h"
#include "CANBuffer.h"
#include "Servo.h"
#include "PwmOut.h"

#define TIME_ERROR_FACTOR 3.15776

// Init. Components

// Servos
Servo panServo(SRVO_PWM_CR, Servo::CONT_SERVO, 44.0, 2.1, 0.9); //44 RPM at 4.8V, max->2100us PW, min->900us PW
Servo tiltServo(SRVO_PWM_MG, Servo::LIM_SERVO, 180, 2.1, 0.9);
/*
// Motors
Motor panServoMotor(GimbConfig::panMotorConfig, Motor::cont_servo);
Motor tiltServoMotor(GimbConfig::tiltMotorConfig, Motor::lim_servo);
*/
// Encoders
EncoderAbsolute_PWM panEncoder(GimbConfig::panEncoderConfig);

// Limit switche
DigitalIn tiltLimUp(LIM_GIMB);

/*
// Rotary actuator
ActuatorController panActuator(GimbConfig::panActuatorConfig, panServoMotor, panEncoder);
ActuatorController tiltActuator(GimbConfig::tiltActuatorConfig, tiltServoMotor, panEncoder); // panEncoder as placeholder, will not be called.
*/

CAN can1(CAN1_RX, CAN1_TX, ROVER_CANBUS_FREQUENCY);
Serial pc (PC_10, PC_11, 8000);

/*
void rxCANProcessor() {
    CANMsg rxMsg;

    while(true) {
        if (can1.read(rxMsg)) {
            float val;
            ActuatorController::t_actuatorControlMode mode;

            switch (rxMsg.id)
            {
            case SET_GIMBAL_PAN_POS :
                if(panActuator.getControlMode() == ActuatorController::position) {
                    rxMsg.getPayload(val);
                    panActuator.setMotionData(val);                    
                }

                break;
            
            case SET_GIMBAL_PAN_SPEED :
                if(panActuator.getControlMode() == ActuatorController::velocity) {
                    rxMsg.getPayload(val);
                    panActuator.setMotionData(val);    
                }
                
                break;
            
            case SET_GIMBAL_PITCH_POS :
                rxMsg.getPayload(val);
                tiltActuator.setMotionData(val);
                break;

            case SET_GIMBAL_PAN_MODE :
                rxMsg.getPayload(mode);
                panActuator.setControlMode(mode);
                break;

            default :
                break;
            }
        }

        ThisThread::sleep_for(1000);
    }
}



void move() {
    panActuator.update();
    tiltActuator.update();
    ThisThread::sleep_for(10);
}

*/

Thread rxCANProcessorThread;
Thread moveThread;


int main()
{
    wait_ms(100);
    printf("\r\n\r\n");
    printf("GIMBAL APPLICATION STARTED \r\n");
    printf("==========================");

/*
    rxCANProcessorThread.set_priority(osPriorityNormal);
    moveThread.set_priority(osPriorityAboveNormal);

    rxCANProcessorThread.start(rxCANProcessor);
    moveThread.start(move);
*/

    PwmOut servo (SRVO_PWM_MG);
    servo.period_ms(1000*TIME_ERROR_FACTOR);
    servo.write(0.5);

    DigitalOut temp(SRVO_PWM_CR);
    while(1){
        printf("1");
        wait_ms(100);
        /*
        if(temp.read()==1)
            temp.write(0);
        else
            temp.write(1);
            */
    }
}
