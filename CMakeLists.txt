cmake_minimum_required(VERSION 3.16.5)

set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

set(CMAKE_VERBOSE_MAKEFILE on)
include(toolchain.cmake) # TODO(melvinw): this should not be included here. It should be configured by ide or by passing it to cmake

# Toolchain must be included before project declaration
project(MarsRoverFirmware)

# Global Settings
set(COMPILE_FLAGS
        -MMD
        -MP

        -O3
        -g

        -Wall
        -Wextra
        -Wpedantic
        -Wvla
        -Werror

        -Wno-unused-parameter
        -Wno-missing-field-initializers

        -ffunction-sections
        -fdata-sections
        -fomit-frame-pointer
        -funsigned-char

        -fno-exceptions
        -fno-delete-null-pointer-checks

        -fstack-protector-strong

        $<$<COMPILE_LANGUAGE:ASM>:-xassembler-with-cpp>
        $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>
        $<$<COMPILE_LANGUAGE:C,CXX>:-include${CMAKE_SOURCE_DIR}/config/mbed_config.h>
        $<$<COMPILE_LANGUAGE:C,CXX>:-include${CMAKE_SOURCE_DIR}/config/rover_config.h>
        $<$<COMPILE_LANGUAGE:C,CXX>:-include${CMAKE_SOURCE_DIR}/config/can_config.h>
        )
add_compile_options(${PROCESSOR_FLAGS} ${COMPILE_FLAGS})

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include mbed-os
include(mbed-os.cmake)

# Include library CMakeLists
add_subdirectory(lib/can)
add_subdirectory(lib/controllers)
add_subdirectory(lib/miscellaneous)
add_subdirectory(lib/neopixel)
add_subdirectory(lib/pid)
add_subdirectory(lib/sensors)
add_subdirectory(lib/servo)

# TODO(wmmc88): check if its in supported configuration list and warn if not in list. This should be in wrapper makefile
if (NOT DEFINED APP)
    message(FATAL_ERROR "APP variable not set in CACHE or ENV")
elseif (NOT EXISTS "${CMAKE_SOURCE_DIR}/apps/${APP}")
    message(FATAL_ERROR "${APP} app does not exist in the apps folder")
endif ()

if (NOT DEFINED TARGET)
    message(FATAL_ERROR "TARGET variable not set in CACHE or ENV")
elseif (NOT EXISTS "${CMAKE_SOURCE_DIR}/targets/${TARGET}")
    message(FATAL_ERROR "${TARGET} target does not exist in the targets folder")
endif ()
message(STATUS "Configuring and generating build for ${APP} app on ${TARGET} target")

# Add CMakeList for target-specific files
add_subdirectory(targets/${TARGET})

# Include Rover App CMakeLists
add_subdirectory(apps/arm)
add_subdirectory(apps/gimbtonomy)
add_subdirectory(apps/safety)
add_subdirectory(apps/science)

# Include Test App CMakeLists
add_subdirectory(apps/test-actuator-controller)
add_subdirectory(apps/test-aeat-8800)
add_subdirectory(apps/test-blinky)
add_subdirectory(apps/test-blockingneo)
add_subdirectory(apps/test-can)
add_subdirectory(apps/test-enc-abs-pwm)
#add_subdirectory(apps/test-moisture)
#add_subdirectory(apps/test-pid)
#add_subdirectory(apps/test-pwm)
#add_subdirectory(apps/test-pwmin)
#add_subdirectory(apps/test-servo)
