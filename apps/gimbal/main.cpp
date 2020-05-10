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

#define POS_TOL 1

// Init. Components
// Servos
Servo panServo(SRVO_PWM_CR, Servo::CONT_SERVO, 44.0, 2.1, 0.9); //44 RPM at 4.8V, max->2100us PW, min->900us PW
Servo pitchServo(SRVO_PWM_MG, Servo::LIM_SERVO, 180, 2.1, 0.9);

EncoderAbsolute_PWM panEncoder(GimbConfig::panEncoderConfig);

// Limit switch
DigitalIn tiltLimUp(LIM_GIMB);

// CAN Object
CAN can1(CAN1_RX, CAN1_TX, ROVER_CANBUS_FREQUENCY);

// RTOS Queue
Queue<float, 3> pos_data;

// CAN LED Debugger
DigitalOut can_debug(LED3);

// Threads
Thread rxCANProcessorThread;
Thread moveThread;

// CAN Message Processor 
void rxCANProcessor() {
    CANMsg rxMsg;

    // Position
    float pan_pos = 0, 
        pan_speed = 0,
        pitch_pos = 0;

    while(true) {
        if (can1.read(rxMsg)) {
            switch (rxMsg.id)
            {
            case SET_GIMBAL_PAN_POS :
                rxMsg.getPayload(pan_pos);
                can_debug.write(1);
                break;
            
            case SET_GIMBAL_PAN_SPEED :
                rxMsg.getPayload(pan_speed);    
                break;
            
            case SET_GIMBAL_PITCH_POS :
                rxMsg.getPayload(pitch_pos);
                break;

            default :
                break;
            }
        }

        pos_data.put(&pan_pos);
        pos_data.put(&pan_speed);
        pos_data.put(&pitch_pos);

        moveThread.signal_set(0x00);
        ThisThread::sleep_for(5); // Sleep CAN processor for 5 ms
    }
}

// Move servos to determined locations

void move() {
    while(true) {
        Thread::signal_wait(0x00); // moveThread does not sleep, but only executes if signaled by can processor
    
        // Set pitch pos
        pitchServo.setPosition(*((float*)(pos_data.get().value.p))); // Cast float to evt ptr and dereference float ptr

        // Set pan speed
        panServo.setSpeed(*((float*)(pos_data.get().value.p))); // Cast float to evt ptr and dereference float ptr

        // Set pan pos - stops panServo once it is within tolerance of set position while traveling at designated speed
        float target_pan_pos = (*((float*)(pos_data.get().value.p))),
            cur_pan_pos = panEncoder.getAngle_Degrees();

        if((cur_pan_pos >= target_pan_pos - POS_TOL) && (cur_pan_pos <= target_pan_pos + POS_TOL))
            panServo.setSpeed(0);
    }
}


int main()
{ 
    rxCANProcessorThread.start(rxCANProcessor);
    moveThread.start(move);
    
    rxCANProcessorThread.set_priority(osPriorityAboveNormal);
    moveThread.set_priority(osPriorityNormal);
    while(1){
        // Output info over serial
        printf("%f \r\n", pitchServo.read());
        printf("%f \r\n", panServo.read());
        ThisThread::sleep_for(100);
    }
}