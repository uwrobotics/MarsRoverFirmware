#include "mbed.h"
#include "mbed_config.h"
#include "can_config.h"

#include "ArmConfig.h"
#include "Encoder.h"
#include "EncoderAbsolute_PWM.h"
#include "EncoderRelative_Quadrature.h"
#include "ActuatorController.h"
#include "DifferentialWristController.h"
#include "ClawController.h"
#include "CANMsg.h"
#include "CANBuffer.h"
#include "Servo.h"

DigitalOut led1(LED1);

CAN can(CAN1_RX, CAN1_TX, ROVER_CANBUS_FREQUENCY);

const int k_interval_ms = 500;


int main() {
  while (true) {
    led1 = !led1;
    wait_ms(k_interval_ms);
  }

}
