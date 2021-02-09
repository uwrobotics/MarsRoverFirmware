/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "CANBus.h"
#include "CANMsg.h"
#include "Neopixel_Blocking.h"
#include "PanConfig.h"
#include "PitchConfig.h"
#include "RollConfig.h"
#include "hw_bridge.h"

#define DEBUG

#define ACK_FLAG (1UL << 0)

// Init. Components
// Servos
/*
• Pan: HS-1425 CR (https://www.robotshop.com/en/hitec-hsr-1425cr-continuous-rotation-servo.html)
• Pitch: HS-422 (https://www.robotshop.com/en/hitec-hs-422-servo-motor.html)
• Roll: SG90 (https://datasheetspdf.com/pdf/791970/TowerPro/SG90/1)
*/
#if false
Actuator::ContServo panServoMotor(SRVO_PWM_CR, 38, 2.1ms,
                                  0.9ms);  // 38 RPM (228 deg/sec) at 4.8V, max->2100us PW, min->900us PW.
Actuator::LimServo pitchServo(SRVO_PWM_HS, 180, 2.1ms, 0.9ms);
/* @TODO: electrical hasn't choose a pin for this servo yet, I'm just using a random free pin for this for now*/
Actuator::LimServo rollServo(SRVO_PWM_SG, 180, 2ms, 1ms);

// Absolute encoder
Encoder::MAE3 panEncoder(GimbtonomyConfig::panEncoderConfig);

// Actuators
Controller::ActuatorController panServoActuator(GimbtonomyConfig::panServoActuatorConfig, panServoMotor,
                                                panEncoder);
#endif
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
      printf("Neo pixels tried to be set to unknow mode\r\n");
      break;
  }
}

// Incoming message processor
void rxCANProcessor() {
  const auto rxPeriod = 2ms;

  float data = 0;
  HWBRIDGE::CONTROL::Mode controlMode;

  while (true) {
    if (can1.read(rxMsg)) {
      switch (rxMsg.getID()) {
        case HWBRIDGE::CANID::SET_PAN_MOTION_DATA:
          rxMsg.getPayload(data);
          Pan::manager.getActiveController()->setSetPoint(data);
          break;
        case HWBRIDGE::CANID::SET_PITCH_MOTION_DATA:
          rxMsg.getPayload(data);
          Pitch::pitchServo.setValue(data);
          break;
        case HWBRIDGE::CANID::SET_ROLL_MOTION_DATA:
          rxMsg.getPayload(data);
          Roll::rollServo.setValue(data);
          break;
        case HWBRIDGE::CANID::SET_PAN_CONTROL_MODE:
          rxMsg.getPayload(controlMode);
          Pan::manager.switchControlMode(controlMode);
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
    float speed = 0;
    if (Pan::manager.getActiveController()->reportAngularVelocityDegPerSec(speed)) {
      printf("panServo(speed): %f \r\n", speed);
    } else {
      printf("panServo(speed): READ ERROR \r\n");
    }
#endif
    Pan::manager.getActiveController()->update();

    ThisThread::sleep_for(1ms);
  }
}
