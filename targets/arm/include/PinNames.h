/* mbed Microcontroller Library
 *******************************************************************************
 * Copyright (c) 2018, STMicroelectronics
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of STMicroelectronics nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************
 */

#pragma once

#include "PinNamesTypes.h"
#include "cmsis.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { ALT0 = 0x100, ALT1 = 0x200, ALT2 = 0x300, ALT3 = 0x400 } ALTx;

typedef enum {
  PA_0       = 0x00,
  PA_0_ALT0  = PA_0 | ALT0,
  PA_0_ALT1  = PA_0 | ALT1,
  PA_1       = 0x01,
  PA_1_ALT0  = PA_1 | ALT0,
  PA_1_ALT1  = PA_1 | ALT1,
  PA_2       = 0x02,
  PA_3       = 0x03,
  PA_4       = 0x04,
  PA_4_ALT0  = PA_4 | ALT0,
  PA_5       = 0x05,
  PA_5_ALT0  = PA_5 | ALT0,
  PA_6       = 0x06,
  PA_6_ALT0  = PA_6 | ALT0,
  PA_7       = 0x07,
  PA_7_ALT0  = PA_7 | ALT0,
  PA_7_ALT1  = PA_7 | ALT1,
  PA_7_ALT2  = PA_7 | ALT2,
  PA_8       = 0x08,
  PA_9       = 0x09,
  PA_10      = 0x0A,
  PA_11      = 0x0B,
  PA_12      = 0x0C,
  PA_13      = 0x0D,
  PA_14      = 0x0E,
  PA_15      = 0x0F,
  PA_15_ALT0 = PA_15 | ALT0,

  PB_0       = 0x10,
  PB_0_ALT0  = PB_0 | ALT0,
  PB_0_ALT1  = PB_0 | ALT1,
  PB_1       = 0x11,
  PB_1_ALT0  = PB_1 | ALT0,
  PB_1_ALT1  = PB_1 | ALT1,
  PB_2       = 0x12,
  PB_3       = 0x13,
  PB_3_ALT0  = PB_3 | ALT0,
  PB_4       = 0x14,
  PB_4_ALT0  = PB_4 | ALT0,
  PB_4_ALT1  = PB_4 | ALT1,
  PB_5       = 0x15,
  PB_5_ALT0  = PB_5 | ALT0,
  PB_5_ALT1  = PB_5 | ALT1,
  PB_6       = 0x16,
  PB_7       = 0x17,
  PB_8       = 0x18,
  PB_8_ALT0  = PB_8 | ALT0,
  PB_8_ALT1  = PB_8 | ALT1,
  PB_9       = 0x19,
  PB_9_ALT0  = PB_9 | ALT0,
  PB_9_ALT1  = PB_9 | ALT1,
  PB_10      = 0x1A,
  PB_12      = 0x1C,
  PB_13      = 0x1D,
  PB_14      = 0x1E,
  PB_14_ALT0 = PB_14 | ALT0,
  PB_14_ALT1 = PB_14 | ALT1,
  PB_15      = 0x1F,
  PB_15_ALT0 = PB_15 | ALT0,
  PB_15_ALT1 = PB_15 | ALT1,

  PC_0       = 0x20,
  PC_0_ALT0  = PC_0 | ALT0,
  PC_0_ALT1  = PC_0 | ALT1,
  PC_1       = 0x21,
  PC_1_ALT0  = PC_1 | ALT0,
  PC_1_ALT1  = PC_1 | ALT1,
  PC_2       = 0x22,
  PC_2_ALT0  = PC_2 | ALT0,
  PC_2_ALT1  = PC_2 | ALT1,
  PC_3       = 0x23,
  PC_3_ALT0  = PC_3 | ALT0,
  PC_3_ALT1  = PC_3 | ALT1,
  PC_4       = 0x24,
  PC_4_ALT0  = PC_4 | ALT0,
  PC_5       = 0x25,
  PC_5_ALT0  = PC_5 | ALT0,
  PC_6       = 0x26,
  PC_6_ALT0  = PC_6 | ALT0,
  PC_7       = 0x27,
  PC_7_ALT0  = PC_7 | ALT0,
  PC_8       = 0x28,
  PC_8_ALT0  = PC_8 | ALT0,
  PC_9       = 0x29,
  PC_9_ALT0  = PC_9 | ALT0,
  PC_10      = 0x2A,
  PC_10_ALT0 = PC_10 | ALT0,
  PC_11      = 0x2B,
  PC_11_ALT0 = PC_11 | ALT0,
  PC_12      = 0x2C,
  PC_13      = 0x2D,
  PC_14      = 0x2E,
  PC_15      = 0x2F,

  PD_0  = 0x30,
  PD_1  = 0x31,
  PD_2  = 0x32,
  PD_3  = 0x33,
  PD_4  = 0x34,
  PD_5  = 0x35,
  PD_6  = 0x36,
  PD_7  = 0x37,
  PD_8  = 0x38,
  PD_9  = 0x39,
  PD_10 = 0x3A,
  PD_11 = 0x3B,
  PD_12 = 0x3C,
  PD_13 = 0x3D,
  PD_14 = 0x3E,
  PD_15 = 0x3F,

  PE_0  = 0x40,
  PE_1  = 0x41,
  PE_2  = 0x42,
  PE_3  = 0x43,
  PE_4  = 0x44,
  PE_5  = 0x45,
  PE_6  = 0x46,
  PE_7  = 0x47,
  PE_8  = 0x48,
  PE_9  = 0x49,
  PE_10 = 0x4A,
  PE_11 = 0x4B,
  PE_12 = 0x4C,
  PE_13 = 0x4D,
  PE_14 = 0x4E,
  PE_15 = 0x4F,

  PH_0 = 0x70,
  PH_1 = 0x71,

  // ADC internal channels
  ADC_TEMP = 0xF0,
  ADC_VREF = 0xF1,
  ADC_VBAT = 0xF2,

// STDIO for console print
#ifdef MBED_CONF_TARGET_STDIO_UART_TX
  STDIO_UART_TX = MBED_CONF_TARGET_STDIO_UART_TX,
#else
  STDIO_UART_TX = PE_8,
#endif
#ifdef MBED_CONF_TARGET_STDIO_UART_RX
  STDIO_UART_RX = MBED_CONF_TARGET_STDIO_UART_RX,
#else
  STDIO_UART_RX = PE_7,
#endif

  /**** UI ****/
  LED1 = PC_14,
  LED2 = PC_15,
  LED3 = PC_5,
  LED4 = PE_15,

  BUTTON_1 = PE_3,
  BUTTON_2 = PC_13,

  /**** Actuator Pins ****/
  MTR_PWM_TRNTBL = PD_14,
  MTR_DIR_TRNTBL = PD_10,

  MTR_PWM_SHLDR = PD_13,
  MTR_DIR_SHLDR = PD_9,

  MTR_PWM_ELBW = PD_15,
  MTR_DIR_ELBW = PD_11,

  MTR_PWM_WRST_LHS = PB_8,
  MTR_DIR_WRST_LHS = PE_0,

  MTR_PWM_WRST_RHS = PB_9,
  MTR_DIR_WRST_RHS = PE_1,

  MTR_PWM_CLAW = PD_12,
  MTR_DIR_CLAW = PD_8,

  SRVO_PWM_CLAW = PA_7,

  /**** Limit Switch Pins ****/
  LIM_TRNTBL_LHS = PD_2,
  LIM_TRNTBL_RHS = PD_3,

  LIM_ELBW_UP = PD_0,
  LIM_ELBW_DN = PD_7,

  LIM_WRST_UP   = PD_4,
  LIM_WRST_CNTR = PD_5,
  LIM_WRST_DN   = PD_6,

  LIM_CLAW_OPEN = PC_12,

  /**** Encoder Pins ****/
  ENC_PWM_TRNTBL = PB_14,
  ENC_PWM_SHLDR  = PC_6,
  ENC_PWM_ELBW   = PE_5,

  ENC_QUAD_CLAW_A = PE_9,
  ENC_QUAD_CLAW_B = PE_11,

  ENC_QUAD_WRST_LHS_A = PA_0,
  ENC_QUAD_WRST_LHS_B = PA_1,

  ENC_QUAD_WRST_RHS_A = PB_4,
  ENC_QUAD_WRST_RHS_B = PC_7,

  /**** Other Sensor Pins ****/
  FORCE_CLAW = PC_1,

  /**** Serial Interface Pins ****/
  CAN1_TX = PA_12,
  CAN1_RX = PA_11,

  CAN2_TX = PB_13,
  CAN2_RX = PB_12,

  CAN_TX = CAN1_TX,
  CAN_RX = CAN1_RX,

  SPI1_SCK  = PB_3,
  SPI1_MOSI = PB_5,
  SPI1_MISO = PA_6,

  SPI2_SCK  = PB_10,
  SPI2_MOSI = PC_3,
  SPI2_MISO = PC_2,

  SPI4_SCK  = PE_2,
  SPI4_MOSI = PE_6,
  SPI4_MISO = PE_13,

  SERIAL_TX  = STDIO_UART_TX,
  SERIAL_RX  = STDIO_UART_RX,
  SERIAL_RTS = PC_8,

  USBTX = STDIO_UART_TX,
  USBRX = STDIO_UART_RX,

  /**** OSCILLATOR pins ****/
  RCC_OSC32_IN  = PC_14,
  RCC_OSC32_OUT = PC_15,
  RCC_OSC_IN    = PH_0,
  RCC_OSC_OUT   = PH_1,

  /**** DEBUG pins ****/
  SYS_JTCK_SWCLK = PA_14,
  SYS_JTDI       = PA_15,
  SYS_JTDO_SWO   = PB_3,
  SYS_JTMS_SWDIO = PA_13,
  SYS_JTRST      = PB_4,
  SYS_TRACED0    = PC_8,
  SYS_WKUP0      = PA_0,
  SYS_WKUP1      = PC_13,

  // Not connected
  NC = (int)0xFFFFFFFF
} PinName;

#ifdef __cplusplus
}
#endif
