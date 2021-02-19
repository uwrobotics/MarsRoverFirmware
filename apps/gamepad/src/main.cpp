/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include <string.h>

#include "AnalogInputGroup.h"
#include "DigitalInputGroup.h"
#include "FrameArbiter.h"
#include "Logger.h"
#include "mbed.h"

using namespace FrameProtocol;

#define DEBUG

static constexpr auto SEND_INTERVAL                = 100ms;
static constexpr uint8_t DEBOUNCE_THRES            = 3;
static constexpr uint8_t ANALOG_NUM_SAMPLE_AVERAGE = 5;

static constexpr uint8_t NUM_BTNS     = 3;
static constexpr uint8_t NUM_SWITCHES = 4;
static constexpr uint8_t NUM_JOYS     = 4;
static constexpr uint8_t NUM_POTS     = 2;

BusIn digital_inputs_bus_btns(JOY_BTN, PB1, PB2);
BusIn digital_inputs_bus_switches(SW_1A, SW_1B, SW_2A, SW_2B);

// we can tune these parameters while testing
DigitalInputGroup btns(digital_inputs_bus_btns, NUM_BTNS, InputDebounceType::INTERGRATING, DEBOUNCE_THRES);
DigitalInputGroup switches(digital_inputs_bus_switches, NUM_SWITCHES, InputDebounceType::INTERGRATING, DEBOUNCE_THRES);

FrameArbiter::DigitalFrameConfig btns_frame_config = {
    .boardType  = BoardType::MASTER,
    .inputType  = DigitalInputType::BTN,
    .inputGroup = btns,
};

FrameArbiter::DigitalFrameConfig switches_frame_config = {
    .boardType  = BoardType::MASTER,
    .inputType  = DigitalInputType::SWITCH,
    .inputGroup = switches,
};

AnalogBusIn analog_inputs_bus_joys(JOY_SM_X, JOY_SM_Y, JOY_X, JOY_Y);
AnalogBusIn analog_inputs_bus_pots(POT_AL, SLIDE_POT_AL);

AnalogInputGroup joys(analog_inputs_bus_joys, NUM_JOYS, AveragingAlgoType::SMA, ANALOG_NUM_SAMPLE_AVERAGE);
AnalogInputGroup pots(analog_inputs_bus_pots, NUM_POTS, AveragingAlgoType::SMA, ANALOG_NUM_SAMPLE_AVERAGE);

FrameArbiter::AnalogFrameConfig joys_frame_config = {
    .boardType  = BoardType::MASTER,
    .inputType  = AnalogInputType::JOY,
    .inputGroup = joys,
};

FrameArbiter::AnalogFrameConfig pots_frame_config = {
    .boardType  = BoardType::MASTER,
    .inputType  = AnalogInputType::POT,
    .inputGroup = pots,
};

Timer timer;

// Serial object for sending data to PC
UnbufferedSerial pc(USBTX, USBRX);

FrameArbiter frame_controller(&pc);

int main() {
  Utility::Logger::printf("Beginning robot gamepad fw app.\r\n");

  // set flow control, as we will be sending a lot stuffs
  pc.set_flow_control(SerialBase::RTSCTS, USB_RTS, USB_CTS);

#ifdef DEBUG
  uint16_t btns_invalid_reads = 0xFFFF, switches_invalid_reads = 0xFFFF;
  uint16_t btns_values = 0, switches_values = 0;
  float joys_values[NUM_JOYS] = {0};
  float pots_values[NUM_POTS] = {0};
#endif

  timer.start();

  while (1) {
    auto start_time = timer.elapsed_time();

    // Assume all inputs during SEND_INTERVAL is unchanging,
    // i.e. all inputs in this period is accumulated
    while (!DigitalInputGroup::interval_passed(start_time, timer.elapsed_time(), SEND_INTERVAL)) {
      // May need to implement these reads as threads, needs testing
      btns.read();
      switches.read();
      joys.read();
      pots.read();
    }

#ifndef DEBUG
    frame_controller.sendFrame(btns_frame_config);
    frame_controller.sendFrame(switches_frame_config);
    frame_controller.sendFrame(joys_frame_config);
    frame_controller.sendFrame(pots_frame_config);
#else
    btns_invalid_reads     = btns.getValuesAndInvalidReads(btns_values);
    switches_invalid_reads = btns.getValuesAndInvalidReads(switches_values);

    joys.getValues(joys_values);
    pots.getValues(pots_values);

    Utility::Logger::printf("Digital Inputs: JOY_BTN: %d, valid: %d", btns_values & 0x1, btns_invalid_reads & 0x1);
    Utility::Logger::printf("Digital Inputs: PB1: %d, valid: %d", (btns_values & 0x1 << 1) >> 1,
                            (btns_invalid_reads & 0x1 << 1) >> 1);
    Utility::Logger::printf("Digital Inputs: PB2: %d, valid: %d", (btns_values & 0x1 << 2) >> 2,
                            (btns_invalid_reads & 0x1 << 2) >> 2);
    Utility::Logger::printf("Digital Inputs: SW_1A: %d, valid: %d", switches_values & 0x1,
                            switches_invalid_reads & 0x1);
    Utility::Logger::printf("Digital Inputs: SW_1B: %d, valid: %d", (switches_values & 0x1 << 1) >> 1,
                            (switches_invalid_reads & 0x1 << 1) >> 1);
    Utility::Logger::printf("Digital Inputs: SW_2A: %d, valid: %d", (switches_values & 0x1 << 2) >> 2,
                            (switches_invalid_reads & 0x1 << 2) >> 2);
    Utility::Logger::printf("Digital Inputs: SW_2B: %d, valid: %d", (switches_values & 0x1 << 3) >> 3,
                            (switches_invalid_reads & 0x1 << 3) >> 3);

    Utility::Logger::printf("Digital Inputs: JOY_SM_X: %f", joys_values[0]);
    Utility::Logger::printf("Digital Inputs: JOY_SM_Y: %f", joys_values[1]);
    Utility::Logger::printf("Digital Inputs: JOY_X: %f", joys_values[2]);
    Utility::Logger::printf("Digital Inputs: JOY_Y: %f", joys_values[3]);
    Utility::Logger::printf("Digital Inputs: POT_AL: %f", pots_values[0]);
    Utility::Logger::printf("Digital Inputs: SLIDE_POT_AL: %f", pots_values[1]);

#endif
  }

  return 1;
}
