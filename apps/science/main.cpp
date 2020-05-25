#include "mbed.h"
#include "mbed_config.h"
#include "can_config.h"

#include "ScienceConfig.h"
#include "Encoder.h"
#include "EncoderAbsolute_PWM.h"
#include "EncoderRelative_Quadrature.h"
#include "ActuatorController.h"
#include "DifferentialWristController.h"
#include "ClawController.h"
#include "CANMsg.h"
#include "CANBuffer.h"
#include "Servo.h"
#include "MoistureSensor.h"

//motors
Motor indexerMotor(MTR_PWM_1, MTR_DIR_1, false);
Motor elevatorMotor(MTR_PWM_2, MTR_DIR_2, false);

//Servo
Servo coverServo(SRVO_PWM_1, ScienceConfig::coverServoType, ScienceConfig::coverServoRange, ScienceConfig::coverServoMaxPulse, ScienceConfig::coverServoMinPulse);
Servo diggerServo(SRVO_PWM_2, ScienceConfig::diggerServoType, ScienceConfig::diggerServoRange, ScienceConfig::diggerServoMaxPulse, ScienceConfig::diggerServoMinPulse);

//encoders
EncoderAbsolute_PWM elevatorEncoder(ScienceConfig::elevatorEncoderConfig);
EncoderRelative_Quadrature centrifugeEncoder(ScienceConfig::centrifugeEncoderConfig);
//limit switches

DigitalIn centrifugeLimFront(LIM_SW_1);
DigitalIn centrifugeLimBack(LIM_SW_2);
DigitalIn liftLimTop(LIM_SW_3);
DigitalIn liftLimBottom(LIM_SW_4);

//I2C

MoistureSensor sensor = MoistureSensor(TEMP_MOIST_I2C_SDA, TEMP_MOIST_I2C_SCL);

DigitalOut led1(LED1);
DigitalOut ledR(LED_R);
DigitalOut ledG(LED_G);
DigitalOut ledB(LED_B);

CAN can(CAN1_RX, CAN1_TX, ROVER_CANBUS_FREQUENCY);

static mbed_error_status_t setMotionData(CANMsg &msg)
{
    float motionData;
    msg.getPayload(motionData);

    switch (msg.id)
    {

    case CANID::SET_COVER_POS:
        return coverServo.set_position(motionData);
    case CANID::SET_DIGGER_POS:
        return diggerServo.set_position(motionData);
    default:
        return MBED_ERROR_INVALID_ARGUMENT;
    }
}
const int k_interval_ms = 500;

int main()
{
    while (true)
    {
        led1 = !led1;
        wait_ms(k_interval_ms);
    }
}
