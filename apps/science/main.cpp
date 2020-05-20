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

CAN can(CAN1_RX, CAN1_TX, ROVER_CANBUS_FREQUENCY);

const int k_interval_ms = 500;

int main()
{
    while (true)
    {
        led1 = !led1;
        wait_ms(k_interval_ms);
    }
}
