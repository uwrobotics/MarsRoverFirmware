/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ActuatorController.h"
#include "CANMsg.h"
#include "ContServo.h"
#include "Encoder.h"
#include "EncoderAbsolute_PWM.h"
#include "LimServo.h"
#include "Neopixel_Blocking.h"
#include "can_config.h"
#include "mbed.h"
#include "mbed_config.h"
#include "rover_config.h"

#define POS_TOL   1
#define MOVE_FLAG (1UL << 0)

// Init. Components
// Servos
ContServo panServo(SRVO_PWM_CR, 44.0, 2.1, 0.9);  // 44 RPM at 4.8V, max->2100us PW, min->900us PW
LimServo pitchServo(SRVO_PWM_MG, 180, 2.1, 0.9);

Encoder::t_encoderConfig panEncoderConfig{ENC_PWM_GIMB, NC, NC, NC, NC, NC, NC, 360.0, 0, Encoder::x2_encoding};
EncoderAbsolute_PWM panEncoder(panEncoderConfig);

// Limit switch
DigitalIn tiltLimUp(LIM_GIMB);

// CAN Object
CAN can1(CAN1_RX, CAN1_TX, ROVER_CANBUS_FREQUENCY);
CANMsg rxMsg, txMsg;

// neopixel
Neopixel_Blocking neopixel(16, LED_MTRX);

// RTOS Queue
Queue<float, 3> pos_data;

// CAN LED Debugger
DigitalOut can_debug(LED3);

// Threads
Thread rxCANProcessorThread;
Thread moveThread;

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

// CAN Message Processor
void rxCANProcessor() {
  // Position
  float pan_pos = 0, pan_speed = 0, pitch_pos = 0;

  while (true) {
    if (can1.read(rxMsg)) {
      switch (rxMsg.id) {
        case CANID::SET_GIMBAL_PAN_POS:
          rxMsg.getPayload(pan_pos);
          can_debug.write(1);
          break;

        case CANID::SET_GIMBAL_PAN_SPEED:
          rxMsg.getPayload(pan_speed);
          break;

        case CANID::SET_GIMBAL_PITCH_POS:
          rxMsg.getPayload(pitch_pos);
          break;

        case CANID::SET_NEOPIXEL:
          // Send an acknowledgement CANMsg back to the Jetson
          printf("Sending neopixel acknowledgement message\r\n");
          txMsg.clear();
          txMsg.id = CANID::NEOPIXEL_ACK;
          txMsg << true;
          can1.write(txMsg);
          printf("Updating neo pixels\r\n");
          handleSetNeoPixelColor(&rxMsg);
          break;

        default:
          break;
      }
    }

    pos_data.put(&pan_pos);
    pos_data.put(&pan_speed);
    pos_data.put(&pitch_pos);

    event_flags.set(MOVE_FLAG);  // Set event flag for moveThread to run

    ThisThread::sleep_for(5);  // Sleep CAN processor for 5 ms
  }
}

// Move servos to determined locations

void move() {
  while (true) {
    event_flags.wait_any(MOVE_FLAG);  // moveThread does not sleep, but only executes if signaled by can processor

    // Set pitch pos
    pitchServo.setPosition(*((float *)(pos_data.get().value.p)));  // Cast float to evt ptr and dereference float ptr

    // Set pan speed
    panServo.setSpeed(*((float *)(pos_data.get().value.p)));  // Cast float to evt ptr and dereference float ptr

    // Set pan pos - stops panServo once it is within tolerance of set position while traveling at designated speed
    float target_pan_pos = (*((float *)(pos_data.get().value.p))), cur_pan_pos = panEncoder.getAngle_Degrees();

    if ((cur_pan_pos >= target_pan_pos - POS_TOL) && (cur_pan_pos <= target_pan_pos + POS_TOL)) panServo.setSpeed(0);
  }
}

int main() {
  rxCANProcessorThread.start(rxCANProcessor);
  moveThread.start(move);

  rxCANProcessorThread.set_priority(osPriorityAboveNormal);
  moveThread.set_priority(osPriorityNormal);
  while (true) {
    // Output info over serial
    printf("%f \r\n", pitchServo.read());
    printf("%f \r\n", panServo.read());
    ThisThread::sleep_for(100);
  }
}