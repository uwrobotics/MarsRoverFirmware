cmake_minimum_required(VERSION 3.16.5)

set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

set(CMAKE_VERBOSE_MAKEFILE on)
include(toolchain.cmake)

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

# Include library targets
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

# Add cmakelist for target-specific files
add_subdirectory(targets/${TARGET})

# Rover Apps

add_executable(arm.${TARGET}-board.elf)
target_sources(arm.${TARGET}-board.elf PRIVATE apps/arm/main.cpp)
target_link_libraries(arm.${TARGET}-board.elf PUBLIC mbed-os)
target_precompile_headers(arm.${TARGET}-board.elf REUSE_FROM mbed-os)
set_target_properties(arm.${TARGET}-board.elf PROPERTIES LINK_DEPENDS ${LINKER_SCRIPT_OUTPUT_PATH})
add_dependencies(arm.${TARGET}-board.elf linker-script)

add_executable(gimbtonomy.${TARGET}-board.elf)
target_sources(gimbtonomy.${TARGET}-board.elf PRIVATE apps/gimbtonomy/main.cpp)
target_link_libraries(gimbtonomy.${TARGET}-board.elf PUBLIC mbed-os)
target_precompile_headers(gimbtonomy.${TARGET}-board.elf REUSE_FROM mbed-os)
set_target_properties(gimbtonomy.${TARGET}-board.elf PROPERTIES LINK_DEPENDS ${LINKER_SCRIPT_OUTPUT_PATH})
add_dependencies(gimbtonomy.${TARGET}-board.elf linker-script)

add_executable(safety.${TARGET}-board.elf)
target_sources(safety.${TARGET}-board.elf PRIVATE apps/safety/main.cpp)
target_link_libraries(safety.${TARGET}-board.elf PUBLIC mbed-os)
target_precompile_headers(safety.${TARGET}-board.elf REUSE_FROM mbed-os)
set_target_properties(safety.${TARGET}-board.elf PROPERTIES LINK_DEPENDS ${LINKER_SCRIPT_OUTPUT_PATH})
add_dependencies(safety.${TARGET}-board.elf linker-script)

add_executable(science.${TARGET}-board.elf)
target_sources(science.${TARGET}-board.elf PRIVATE apps/science/main.cpp)
target_link_libraries(science.${TARGET}-board.elf PUBLIC mbed-os)
target_precompile_headers(science.${TARGET}-board.elf REUSE_FROM mbed-os)
set_target_properties(science.${TARGET}-board.elf PROPERTIES LINK_DEPENDS ${LINKER_SCRIPT_OUTPUT_PATH})
add_dependencies(science.${TARGET}-board.elf linker-script)

# Test Apps

add_executable(test-actuator-controller.${TARGET}-board.elf)
target_sources(test-actuator-controller.${TARGET}-board.elf PRIVATE apps/test-actuator-controller/main.cpp)
target_link_libraries(test-actuator-controller.${TARGET}-board.elf PUBLIC mbed-os)
target_precompile_headers(test-actuator-controller.${TARGET}-board.elf REUSE_FROM mbed-os)
set_target_properties(test-actuator-controller.${TARGET}-board.elf PROPERTIES LINK_DEPENDS ${LINKER_SCRIPT_OUTPUT_PATH})
add_dependencies(test-actuator-controller.${TARGET}-board.elf linker-script)

add_executable(test-aeat-8800.${TARGET}-board.elf)
target_sources(test-aeat-8800.${TARGET}-board.elf PRIVATE apps/test-aeat-8800/main.cpp)
target_link_libraries(test-aeat-8800.${TARGET}-board.elf PUBLIC mbed-os)
target_precompile_headers(test-aeat-8800.${TARGET}-board.elf REUSE_FROM mbed-os)
set_target_properties(test-aeat-8800.${TARGET}-board.elf PROPERTIES LINK_DEPENDS ${LINKER_SCRIPT_OUTPUT_PATH})
add_dependencies(test-aeat-8800.${TARGET}-board.elf linker-script)

add_executable(test-blinky.${TARGET}-board.elf)
target_sources(test-blinky.${TARGET}-board.elf PRIVATE apps/test-blinky/main.cpp)
target_link_libraries(test-blinky.${TARGET}-board.elf PUBLIC mbed-os)
target_precompile_headers(test-blinky.${TARGET}-board.elf REUSE_FROM mbed-os)
set_target_properties(test-blinky.${TARGET}-board.elf PROPERTIES LINK_DEPENDS ${LINKER_SCRIPT_OUTPUT_PATH})
add_dependencies(test-blinky.${TARGET}-board.elf linker-script)

add_executable(test-blockingneo.${TARGET}-board.elf)
target_sources(test-blockingneo.${TARGET}-board.elf PRIVATE apps/test-blockingneo/main.cpp)
target_link_libraries(test-blockingneo.${TARGET}-board.elf PUBLIC mbed-os)
target_precompile_headers(test-blockingneo.${TARGET}-board.elf REUSE_FROM mbed-os)
set_target_properties(test-blockingneo.${TARGET}-board.elf PROPERTIES LINK_DEPENDS ${LINKER_SCRIPT_OUTPUT_PATH})
add_dependencies(test-blockingneo.${TARGET}-board.elf linker-script)

add_executable(test-can.${TARGET}-board.elf)
target_sources(test-can.${TARGET}-board.elf PRIVATE apps/test-can/main.cpp)
target_link_libraries(test-can.${TARGET}-board.elf PUBLIC mbed-os)
target_precompile_headers(test-can.${TARGET}-board.elf REUSE_FROM mbed-os)
set_target_properties(test-can.${TARGET}-board.elf PROPERTIES LINK_DEPENDS ${LINKER_SCRIPT_OUTPUT_PATH})
add_dependencies(test-can.${TARGET}-board.elf linker-script)

add_executable(test-enc-abs-pwm.${TARGET}-board.elf)
target_sources(test-enc-abs-pwm.${TARGET}-board.elf PRIVATE apps/test-enc-abs-pwm/main.cpp)
target_link_libraries(test-enc-abs-pwm.${TARGET}-board.elf PUBLIC mbed-os)
target_precompile_headers(test-enc-abs-pwm.${TARGET}-board.elf REUSE_FROM mbed-os)
set_target_properties(test-enc-abs-pwm.${TARGET}-board.elf PROPERTIES LINK_DEPENDS ${LINKER_SCRIPT_OUTPUT_PATH})
add_dependencies(test-enc-abs-pwm.${TARGET}-board.elf linker-script)

add_executable(test-moisture.${TARGET}-board.elf)
target_sources(test-moisture.${TARGET}-board.elf PRIVATE apps/test-moisture/main.cpp)
target_link_libraries(test-moisture.${TARGET}-board.elf PUBLIC mbed-os)
target_precompile_headers(test-moisture.${TARGET}-board.elf REUSE_FROM mbed-os)
set_target_properties(test-moisture.${TARGET}-board.elf PROPERTIES LINK_DEPENDS ${LINKER_SCRIPT_OUTPUT_PATH})
add_dependencies(test-moisture.${TARGET}-board.elf linker-script)

add_executable(test-pid.${TARGET}-board.elf)
target_sources(test-pid.${TARGET}-board.elf PRIVATE apps/test-pid/main.cpp)
target_link_libraries(test-pid.${TARGET}-board.elf PUBLIC mbed-os)
target_precompile_headers(test-pid.${TARGET}-board.elf REUSE_FROM mbed-os)
set_target_properties(test-pid.${TARGET}-board.elf PROPERTIES LINK_DEPENDS ${LINKER_SCRIPT_OUTPUT_PATH})
add_dependencies(test-pid.${TARGET}-board.elf linker-script)

add_executable(test-pwm.${TARGET}-board.elf)
target_sources(test-pwm.${TARGET}-board.elf PRIVATE apps/test-pwm/main.cpp)
target_link_libraries(test-pwm.${TARGET}-board.elf PUBLIC mbed-os)
target_precompile_headers(test-pwm.${TARGET}-board.elf REUSE_FROM mbed-os)
set_target_properties(test-pwm.${TARGET}-board.elf PROPERTIES LINK_DEPENDS ${LINKER_SCRIPT_OUTPUT_PATH})
add_dependencies(test-pwm.${TARGET}-board.elf linker-script)

add_executable(test-pwmin.${TARGET}-board.elf)
target_sources(test-pwmin.${TARGET}-board.elf PRIVATE apps/test-pwmin/main.cpp)
target_link_libraries(test-pwmin.${TARGET}-board.elf PUBLIC mbed-os)
target_precompile_headers(test-pwmin.${TARGET}-board.elf REUSE_FROM mbed-os)
set_target_properties(test-pwmin.${TARGET}-board.elf PROPERTIES LINK_DEPENDS ${LINKER_SCRIPT_OUTPUT_PATH})
add_dependencies(test-pwmin.${TARGET}-board.elf linker-script)

add_executable(test-servo.${TARGET}-board.elf)
target_sources(test-servo.${TARGET}-board.elf PRIVATE apps/test-servo/main.cpp)
target_link_libraries(test-servo.${TARGET}-board.elf PUBLIC mbed-os)
target_precompile_headers(test-servo.${TARGET}-board.elf REUSE_FROM mbed-os)
set_target_properties(test-servo.${TARGET}-board.elf PROPERTIES LINK_DEPENDS ${LINKER_SCRIPT_OUTPUT_PATH})
add_dependencies(test-servo.${TARGET}-board.elf linker-script)
