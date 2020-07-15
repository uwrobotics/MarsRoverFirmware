/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ActuatorController.h"
#include "CANMsg.h"
#include "Encoder.h"
#include "EncoderAbsolute_PWM.h"
#include "GimbtonomyConfig.h"
#include "LimServo.h"
#include "Neopixel_Blocking.h"
#include "ServoMotor.h"
#include "can_config.h"
#include "mbed.h"
#include "mbed_config.h"
#include "rover_config.h"

#define ACK_FLAG (1UL << 0)

// Init. Components
// Servos
ServoMotor panServoMotor(SRVO_PWM_CR, 44.0, 2.1, 0.9);  // 44 RPM (264 deg/sec) at 4.8V, max->2100us PW, min->900us PW
LimServo pitchServo(SRVO_PWM_MG, 180, 2.1, 0.9);

// Absolute encoder
EncoderAbsolute_PWM panEncoder(GimbtonomyConfig::panEncoderConfig);

// Actuators
ActuatorController panServoActuator(GimbtonomyConfig::panServoActuatorConfig, panServoMotor, panEncoder);

// CAN Object
CAN can1(CAN1_RX, CAN1_TX, ROVER_CANBUS_FREQUENCY);
CANMsg rxMsg, txMsg;

// neopixel
Neopixel_Blocking neopixel(16, LED_MTRX);

// Threads
Thread rxCANProcessorThread(osPriorityAboveNormal);
Thread txCANProcessorThread(osPriorityBelowNormal);

// Event flags for communication between threads
EventFlags event_flags;

void handleSetNeoPixelColor(CANMsg *p_newMsg) {
  enum mode : uint8_t { solidRed, solidBlue, flashingGreen, off };
  uint8_t neoPixelMode = 0;
  *p_newMsg >> neoPixelMode;
  switch (neoPixelMode) {
    case solidRed:
      printf("Setting neo pixels to solid red\r\n");
      neopixel.displayRed();
      break;
    case solidBlue:
      printf("Setting neo pixels to solid blue\r\n");
      neopixel.displayBlue();
      break;
    case flashingGreen:
      printf("Setting neo pixels to flashing green\r\n");
      neopixel.blinkPixels(10, 2, neopixel.Green);
      break;
    case off:
      printf("Turning neo pixels off\r\n");
      neopixel.shutdown();
      break;
    default:
      printf("Neo pixels tried to be set to unknow mode\r\n");
      break;
  }
}

// Incoming message processor
void rxCANProcessor() {
  const int rxPeriod_millisec = 2;

  float pan_pos = 0.0, pan_speed = 0.0, pitch_pos = 0.0;

  while (true) {
    if (can1.read(rxMsg)) {
      switch (rxMsg.id) {
        case CANID::SET_GIMBAL_PAN_POS:
          rxMsg.getPayload(pan_pos);
          panServoActuator.setMotionData(pan_pos);
          break;

        case CANID::SET_GIMBAL_PAN_SPEED:
          rxMsg.getPayload(pan_speed);
          panServoMotor.servoSetSpeed(pan_speed);
          break;

        case CANID::SET_GIMBAL_PITCH_POS:
          rxMsg.getPayload(pitch_pos);
          pitchServo.setPosition(pitch_pos);
          break;

        case CANID::SET_NEOPIXEL:
          handleSetNeoPixelColor(&rxMsg);
          // Hangle can writes in a separate thread
          event_flags.set(ACK_FLAG);
          break;

        default:
          break;
      }
    }

    ThisThread::sleep_for(rxPeriod_millisec);
  }
}

// Outgoing message processor
void txCANProcessor() {
  while (true) {
    // This thread does not sleep, but only executes if signaled by rxCANProcessor to write out a response
    event_flags.wait_any(ACK_FLAG);

    // Send an acknowledgement CANMsg back to the Jetson
    printf("Sending neopixel acknowledgement message\r\n");
    txMsg.clear();
    txMsg.id = CANID::NEOPIXEL_ACK;
    txMsg << true;
    can1.write(txMsg);
    printf("Updating neo pixels\r\n");
  }
}

int main() {
  printf("\r\n\r\n");
  printf("ARM APPLICATION STARTED\r\n");
  printf("=======================\r\n");

  rxCANProcessorThread.start(rxCANProcessor);
  txCANProcessorThread.start(txCANProcessor);

  while (true) {
    // Output info over serial
    printf("pitchServo(pos): %f \r\n", pitchServo.read());
    printf("panServo(speed): %f \r\n", panServoMotor.servoRead());

    panServoActuator.update();

    ThisThread::sleep_for(2);
  }
}