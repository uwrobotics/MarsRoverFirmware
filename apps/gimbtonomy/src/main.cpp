/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ActuatorController.h"
#include "CANBus.h"
#include "CANMsg.h"
#include "Encoder.h"
#include "EncoderAbsolute_PWM.h"
#include "GimbtonomyConfig.h"
#include "LimServo.h"
#include "Neopixel_Blocking.h"
#include "ServoMotor.h"
#include "hw_bridge.h"
#include "mbed.h"

#define DEBUG

#define ACK_FLAG (1UL << 0)

// Init. Components
// Servos
/*
• Pan: HS-1425 CR (https://www.robotshop.com/en/hitec-hsr-1425cr-continuous-rotation-servo.html)
• Pitch: HS-422 (https://www.robotshop.com/en/hitec-hs-422-servo-motor.html)
• Roll: SG90 (https://datasheetspdf.com/pdf/791970/TowerPro/SG90/1)
*/
ServoMotor panServoMotor(SRVO_PWM_CR, false, 2.1ms, 0.9ms,
                         38);  // 38 RPM (228 deg/sec) at 4.8V, max->2100us PW, min->900us PW.
LimServo pitchServo(SRVO_PWM_HS, 180, 2.1ms, 0.9ms);
/* @TODO: electrical hasn't choose a pin for this servo yet, I'm just using a random free pin for this for now*/
LimServo rollServo(SRVO_PWM_SG, 180, 2ms, 1ms);

// Absolute encoder
EncoderAbsolute_PWM panEncoder(GimbtonomyConfig::panEncoderConfig);

// Actuators
ActuatorController panServoActuator(GimbtonomyConfig::panServoActuatorConfig, panServoMotor, panEncoder);

// CAN Object
CANBus can1(CAN1_RX, CAN1_TX, HWBRIDGE::ROVERCONFIG::ROVER_CANBUS_FREQUENCY);
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
      neopixel.blinkPixels(10, 2s, neopixel.Green);
      break;
    case off:
      printf("Turning neo pixels off\r\n");
      neopixel.shutdown();
      break;
    default:
      printf("Neo pixels tried to be set to unknown mode\r\n");
      break;
  }
}

// Incoming message processor
void rxCANProcessor() {
  const auto rxPeriod = 2ms;

  float pan_pos = 0.0, pan_speed = 0.0, pitch_pos = 0.0, roll_pos = 0.0;
  ActuatorController::t_actuatorControlMode controlMode;

  while (true) {
    if (can1.read(rxMsg)) {
      switch (rxMsg.getID()) {
        case HWBRIDGE::CANID::GIMBAL_PAN_POSITION:
          rxMsg.getPayload(pan_pos);
          panServoActuator.setMotionData(pan_pos);
          break;

        case HWBRIDGE::CANID::GIMBAL_PAN_SPEED:
          rxMsg.getPayload(pan_speed);
          panServoMotor.servoSetSpeed(pan_speed);
          break;

        case HWBRIDGE::CANID::GIMBAL_PAN_MODE:
          rxMsg.getPayload(controlMode);
          panServoActuator.setControlMode(controlMode);
          break;

        case HWBRIDGE::CANID::GIMBAL_PITCH_POS:
          rxMsg.getPayload(pitch_pos);
          pitchServo.setPosition(pitch_pos);
          break;

        case HWBRIDGE::CANID::GIMBAL_ROLL_POS:
          rxMsg.getPayload(roll_pos);
          rollServo.setPosition(roll_pos);
          break;

        case HWBRIDGE::CANID::NEOPIXEL_SET:
          handleSetNeoPixelColor(&rxMsg);
          // Hangle can writes in a separate thread
          event_flags.set(ACK_FLAG);
          break;

        default:
          break;
      }
    }

    ThisThread::sleep_for(rxPeriod);
  }
}

// Outgoing message processor
void txCANProcessor() {
  while (true) {
    // This thread does not sleep, but only executes if signaled by rxCANProcessor to write out a response
    event_flags.wait_any(ACK_FLAG);

    // Send an acknowledgement CANMsg back to the Jetson
#ifdef DEBUG
    printf("Sending neopixel acknowledgement message\r\n");
#endif
    txMsg.clear();
    txMsg.setID(HWBRIDGE::CANID::NEOPIXEL_ACK);
    txMsg << true;
    can1.write(txMsg);
#ifdef DEBUG
    printf("Updating neo pixels\r\n");
#endif
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
#ifdef DEBUG
    printf("pitchServo(pos): %f \r\n", pitchServo.read());
    printf("rollServo(pos): %f \r\n", rollServo.read());
    printf("panServo(speed): %f \r\n", panServoMotor.servoRead());
#endif
    panServoActuator.update();

    ThisThread::sleep_for(2ms);
  }
}
