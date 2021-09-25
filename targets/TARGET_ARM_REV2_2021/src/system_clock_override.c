/* This file overrides the necessary MBED_WEAK functions from system_clock.c to achieve our desired clock config. The
 * specific version of system_clock.c in the mbed-os repo thats these functions are overriding is based upon the
 * mbed-xxx target (ex. mbed-stm32f446xe) that this target is linked against in the CMakeLists.txt.
 *
 ******************************************************************************
 * mbed Microcontroller Library
 * SPDX-License-Identifier: BSD-3-Clause
 ******************************************************************************
 *
 * Copyright (c) 2015-2021 STMicroelectronics.
 * All rights reserved.
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

#include "stm32f4xx.h"

/******************************************************************************/
/*            PLL (clocked by HSE) used as System clock source                */
/******************************************************************************/
uint8_t SetSysClock_PLL_HSE(uint8_t bypass) {
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

  /* The voltage scaling allows optimizing the power consumption when the device is
     clocked below the maximum system frequency, to update the voltage scaling value
     regarding system frequency refer to product datasheet. */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /* Get the Clocks configuration according to the internal RCC registers */
  HAL_RCC_GetOscConfig(&RCC_OscInitStruct);

  /* PLL could be already configured by bootlader */
  if (RCC_OscInitStruct.PLL.PLLState != RCC_PLL_ON) {
    // Enable HSE oscillator and activate PLL with HSE as source
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    if (bypass == 0) {
      RCC_OscInitStruct.HSEState = RCC_HSE_ON;  // External 8 MHz xtal on OSC_IN/OSC_OUT
    } else {
      RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;  // External 8 MHz clock on OSC_IN
    }

    RCC_OscInitStruct.PLL.PLLState  = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM      = 25;             // VCO input clock = 1 MHz (25 MHz / 25)
    RCC_OscInitStruct.PLL.PLLN      = 360;            // VCO output clock = 360 MHz (1 MHz * 360)
    RCC_OscInitStruct.PLL.PLLP      = RCC_PLLP_DIV2;  // PLLCLK = 180 MHz (360 MHz / 2)
    RCC_OscInitStruct.PLL.PLLQ      = 7;              //
    RCC_OscInitStruct.PLL.PLLR      = 2;              //
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
      return 0;  // FAIL
    }
  }

  // Activate the OverDrive to reach the 180 MHz Frequency
  if (HAL_PWREx_EnableOverDrive() != HAL_OK) {
    return 0;  // FAIL
  }

#if DEVICE_USBDEVICE
  // Select PLLSAI output as USB clock source
  PeriphClkInitStruct.PLLSAI.PLLSAIM       = 25;
  PeriphClkInitStruct.PLLSAI.PLLSAIN       = 384;
  PeriphClkInitStruct.PLLSAI.PLLSAIP       = RCC_PLLSAIP_DIV8;
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_CLK48;
  PeriphClkInitStruct.Clk48ClockSelection  = RCC_CLK48CLKSOURCE_PLLSAIP;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
#endif /* DEVICE_USBDEVICE */

  // Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;  // 180 MHz
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;    //  45 MHz
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;    //  90 MHz
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK) {
    return 0;  // FAIL
  }

// Output clock on MCO1 pin(PA8) for debugging purpose
#if DEBUG_MCO == 1
  if (bypass == 0) {
    HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_HSE, RCC_MCODIV_2);  // 4 MHz with xtal
  } else {
    HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_HSE, RCC_MCODIV_1);  // 8 MHz with external clock (MCO)
  }
#endif

  return 1;  // OK
}
