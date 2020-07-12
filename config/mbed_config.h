/*
 * mbed SDK
 * Copyright (c) 2017 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __MBED_CONFIG_DATA__
#define __MBED_CONFIG_DATA__

#include "mbed_config_target.h"
#include "rover_config.h"

// MBED Debug configuration
#define __MBED__
#define MBED_TRAP_ERRORS_ENABLED 1

// CORTEX Macros
#define ARM_MATH_CM4
#define __CORTEX_M4
#define __CMSIS_RTOS
#define __MBED_CMSIS_RTOS_CM
#define __FPU_PRESENT 1

// ST Macros
#define USE_HAL_DRIVER
#define USE_FULL_LL_DRIVER

// MBED configuration parameters TODO(wmmc88): update based on mbed_lib.jsons
// https://os.mbed.com/docs/mbed-os/v6.0/program-setup/advanced-configuration.html and remove any unused params
#define LPTICKER_DELAY_TICKS                                 1     // set by target:FAMILY_STM32
#define MBED_CONF_DRIVERS_UART_SERIAL_RXBUF_SIZE             256   // set by library:drivers
#define MBED_CONF_DRIVERS_UART_SERIAL_TXBUF_SIZE             256   // set by library:drivers
#define MBED_CONF_EVENTS_PRESENT                             1     // set by library:events
#define MBED_CONF_EVENTS_SHARED_DISPATCH_FROM_APPLICATION    0     // set by library:events
#define MBED_CONF_EVENTS_SHARED_EVENTSIZE                    768   // set by library:events
#define MBED_CONF_EVENTS_SHARED_HIGHPRIO_EVENTSIZE           256   // set by library:events
#define MBED_CONF_EVENTS_SHARED_HIGHPRIO_STACKSIZE           1024  // set by library:events
#define MBED_CONF_EVENTS_SHARED_STACKSIZE                    2048  // set by library:events
#define MBED_CONF_EVENTS_USE_LOWPOWER_TIMER_TICKER           0     // set by library:events
#define MBED_CONF_PLATFORM_CRASH_CAPTURE_ENABLED             0     // set by library:platform
#define MBED_CONF_PLATFORM_CTHUNK_COUNT_MAX                  8     // set by library:platform
#define MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE          ROVER_DEFAULT_SERIAL_BAUD_RATE  // set by library:platform
#define MBED_CONF_PLATFORM_ERROR_ALL_THREADS_INFO            0                               // set by library:platform
#define MBED_CONF_PLATFORM_ERROR_FILENAME_CAPTURE_ENABLED    0                               // set by library:platform
#define MBED_CONF_PLATFORM_ERROR_HIST_ENABLED                0                               // set by library:platform
#define MBED_CONF_PLATFORM_ERROR_HIST_SIZE                   4                               // set by library:platform
#define MBED_CONF_PLATFORM_ERROR_REBOOT_MAX                  1                               // set by library:platform
#define MBED_CONF_PLATFORM_FATAL_ERROR_AUTO_REBOOT_ENABLED   0                               // set by library:platform
#define MBED_CONF_PLATFORM_FORCE_NON_COPYABLE_ERROR          0                               // set by library:platform
#define MBED_CONF_PLATFORM_MAX_ERROR_FILENAME_LEN            16                              // set by library:platform
#define MBED_CONF_PLATFORM_POLL_USE_LOWPOWER_TIMER           0                               // set by library:platform
#define MBED_CONF_PLATFORM_STDIO_BAUD_RATE                   ROVER_DEFAULT_SERIAL_BAUD_RATE  // set by library:platform
#define MBED_CONF_PLATFORM_STDIO_BUFFERED_SERIAL             1                               // set by library:platform
#define MBED_CONF_PLATFORM_STDIO_CONVERT_NEWLINES            1                               // set by library:platform
#define MBED_CONF_PLATFORM_STDIO_CONVERT_TTY_NEWLINES        1                               // set by library:platform
#define MBED_CONF_PLATFORM_STDIO_FLUSH_AT_EXIT               1                               // set by library:platform
#define MBED_CONF_PLATFORM_STDIO_MINIMAL_CONSOLE_ONLY        0                               // set by library:platform
#define MBED_CONF_PLATFORM_USE_MPU                           1                               // set by library:platform
#define MBED_CONF_RTOS_API_PRESENT                           1                               // set by library:rtos
#define MBED_CONF_RTOS_IDLE_THREAD_STACK_SIZE                512                             // set by library:rtos
#define MBED_CONF_RTOS_IDLE_THREAD_STACK_SIZE_DEBUG_EXTRA    128                             // set by library:rtos[STM]
#define MBED_CONF_RTOS_IDLE_THREAD_STACK_SIZE_TICKLESS_EXTRA 256                             // set by library:rtos
#define MBED_CONF_RTOS_MAIN_THREAD_STACK_SIZE                4096                            // set by library:rtos
#define MBED_CONF_RTOS_PRESENT                               1                               // set by library:rtos
#define MBED_CONF_RTOS_THREAD_STACK_SIZE                     4096                            // set by library:rtos
#define MBED_CONF_RTOS_TIMER_THREAD_STACK_SIZE               768                             // set by library:rtos
#define MBED_CONF_TARGET_BOOT_STACK_SIZE                     0x400                           // set by library:rtos[*]
#define MBED_CONF_TARGET_CONSOLE_UART                        1                               // set by target:Target
#define MBED_CONF_TARGET_DEEP_SLEEP_LATENCY                  3     // set by target:FAMILY_STM32
#define MBED_CONF_TARGET_DEFAULT_ADC_VREF                    3.3f  // set by target:FAMILY_STM32
#define MBED_CONF_TARGET_INIT_US_TICKER_AT_BOOT              1     // set by target:FAMILY_STM32
#define MBED_CONF_TARGET_LPTICKER_LPTIM_CLOCK                1     // set by target:FAMILY_STM32
#define MBED_CONF_TARGET_LPUART_CLOCK_SOURCE                 USE_LPUART_CLK_LSE | USE_LPUART_CLK_PCLK1  // set by target:FAMILY_STM32
#define MBED_CONF_TARGET_LSE_AVAILABLE                       0           // set by target:FAMILY_STM32
#define MBED_CONF_TARGET_MPU_ROM_END                         0x0fffffff  // set by target:Target
#define MBED_CONF_TARGET_TICKLESS_FROM_US_TICKER             0           // set by target:Target

// Device driver configuration
#define DEVICE_ANALOGIN       1
#define DEVICE_ANALOGOUT      1
#define DEVICE_CAN            1
#define DEVICE_FLASH          1
#define DEVICE_I2C            1
#define DEVICE_I2C_ASYNCH     1
#define DEVICE_I2CSLAVE       1
#define DEVICE_INTERRUPTIN    1
#define DEVICE_LPTICKER       1
#define DEVICE_MPU            1
#define DEVICE_PORTIN         1
#define DEVICE_PORTINOUT      1
#define DEVICE_PORTOUT        1
#define DEVICE_PWMOUT         1
#define DEVICE_RESET_REASON   1
#define DEVICE_RTC            1
#define DEVICE_SERIAL         1
#define DEVICE_SERIAL_ASYNCH  1
#define DEVICE_SERIAL_FC      1
#define DEVICE_SLEEP          1
#define DEVICE_SPI            1
#define DEVICE_SPI_ASYNCH     1
#define DEVICE_SPISLAVE       1
#define DEVICE_STDIO_MESSAGES 1
#define DEVICE_USTICKER       1
#define DEVICE_WATCHDOG       1

// Macros
#define _RTE_  // defined by library:rtos

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define PRINT_INFO(...)                  \
  {                                      \
    printf("[%s] INFO: ", __FILENAME__); \
    printf(__VA_ARGS__);                 \
  }

#define PRINT_WARNING(...)                  \
  {                                         \
    printf("[%s] WARNING: ", __FILENAME__); \
    printf(__VA_ARGS__);                    \
  }

#define MBED_WARN_ON_ERROR(functionCall)                                                      \
  {                                                                                           \
    mbed_error_status_t result = functionCall;                                                \
    if (result != MBED_SUCCESS) {                                                             \
      PRINT_WARNING("Operation '%s' failed with status code %d \r\n", #functionCall, result); \
    }                                                                                         \
  }

#define MBED_WARN_AND_RETURN_STATUS_ON_ERROR(functionCall)                                    \
  {                                                                                           \
    mbed_error_status_t result = functionCall;                                                \
    if (result != MBED_SUCCESS) {                                                             \
      PRINT_WARNING("Operation '%s' failed with status code %d \r\n", #functionCall, result); \
      return result;                                                                          \
    }                                                                                         \
  }

#define MBED_ASSERT_WARN(assertion)                          \
  {                                                          \
    if ((assertion) == false) {                              \
      PRINT_WARNING("Failed assertion: %s\r\n", #assertion); \
    }                                                        \
  }

#endif
