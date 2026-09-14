cmake_minimum_required(VERSION 3.19.5)

# Activate ccache
find_program(CCACHE "ccache")
if (CCACHE)
    set(CMAKE_C_COMPILER_LAUNCHER ${CCACHE})
    set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE})
endif ()

# Path Variables
set(ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})
set(MBED_PATH ${ROOT_DIR}/mbed-os CACHE INTERNAL "")
set(MBED_CONFIG_PATH ${CMAKE_CURRENT_BINARY_DIR} CACHE INTERNAL "")

# For convenience, define useful variables
set(TARGETS_DIR ${ROOT_DIR}/targets)
set(LIBS_DIR ${ROOT_DIR}/libs)
set(ROVER_APPS_DIR ${ROOT_DIR}/rover-apps)
set(TEST_APPS_DIR ${ROOT_DIR}/test-apps)

# Prevent in-source builds
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

# Force C++ version (hack until https://github.com/ARMmbed/mbed-tools/issues/261 is fixed)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Force extra compile options (hack until https://github.com/ARMmbed/mbed-tools/issues/261 is fixed)
add_compile_options(
        -Wpedantic
        -Wvla
        -Werror)

# Generate compile commands database for other tools to use
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

# Include mbed os cmake logic
include(${MBED_PATH}/tools/cmake/app.cmake)

# Force colored output from compiler
add_compile_options(-fdiagnostics-color)

# Include custom targets
add_subdirectory(${TARGETS_DIR}/TARGET_ARM_REV2_2021)
add_subdirectory(${TARGETS_DIR}/TARGET_GAMEPAD_REV1_2021)
add_subdirectory(${TARGETS_DIR}/TARGET_GIMBAL_REV2_2021)
add_subdirectory(${TARGETS_DIR}/TARGET_PDB_REV2_2021)
add_subdirectory(${TARGETS_DIR}/TARGET_SCIENCE_REV2_2021)
add_subdirectory(${TARGETS_DIR}/TARGET_UWRT_NUCLEO)

# Include mbed-os and disable warnings from within mbed-os
add_subdirectory(${MBED_PATH})
target_compile_options(mbed-os INTERFACE -w) # Disable all warnings from mbed-os core code
target_compile_options(mbed-events PRIVATE -w) # Disable all warnings from mbed-os events code
target_compile_options(mbed-mbedtls PRIVATE -w) # Bug in mbed causes warning due to missing string.h include in error.c
target_compile_options(mbed-nanostack-libservice PRIVATE -Wno-vla)

# Project Name
project(MarsRoverFirmware LANGUAGES C CXX ASM)

# Add HW Bridge
add_subdirectory(uwrt-mars-rover-hw-bridge)

# Include libraries
add_subdirectory(${LIBS_DIR}/actuators)
add_subdirectory(${LIBS_DIR}/can)
add_subdirectory(${LIBS_DIR}/encoder)
add_subdirectory(${LIBS_DIR}/gamepad)
add_subdirectory(${LIBS_DIR}/gpio)
add_subdirectory(${LIBS_DIR}/controllers)
add_subdirectory(${LIBS_DIR}/led-matrix)
add_subdirectory(${LIBS_DIR}/neopixel)
add_subdirectory(${LIBS_DIR}/pid)
add_subdirectory(${LIBS_DIR}/sensors)
add_subdirectory(${LIBS_DIR}/utility)

# Only add subdirectory of requested APP
function(add_app_subdirectory APP_SUBDIRECTORY)
    string(FIND ${APP_SUBDIRECTORY} / INDEX REVERSE)
    math(EXPR INDEX "${INDEX} + 1")
    string(SUBSTRING ${APP_SUBDIRECTORY} ${INDEX} -1 APP_NAME)

    if ("${APP_NAME}" STREQUAL "")
        message(FATAL_ERROR "add_app_subdirectory could not extract an app name from ${APP_SUBDIRECTORY}")
    elseif ("$ENV{APP}" STREQUAL ${APP_NAME})
        add_subdirectory(${APP_SUBDIRECTORY})
    endif ()
endfunction()

## Include Rover Apps
add_subdirectory(${ROVER_APPS_DIR}/common)
add_app_subdirectory(${ROVER_APPS_DIR}/arm_2021)
add_app_subdirectory(${ROVER_APPS_DIR}/gamepad_2021)
add_app_subdirectory(${ROVER_APPS_DIR}/gimbal_2021)
add_app_subdirectory(${ROVER_APPS_DIR}/pdb_2021)
add_app_subdirectory(${ROVER_APPS_DIR}/science_2021)

# Include Test Apps
add_app_subdirectory(${TEST_APPS_DIR}/test-actuator-controller)
add_app_subdirectory(${TEST_APPS_DIR}/test-adafruitSTEMMA)
add_app_subdirectory(${TEST_APPS_DIR}/test-aeat-6012)
add_app_subdirectory(${TEST_APPS_DIR}/test-aeat-8800)
add_app_subdirectory(${TEST_APPS_DIR}/test-blinky)
add_app_subdirectory(${TEST_APPS_DIR}/test-blockingneo)
add_app_subdirectory(${TEST_APPS_DIR}/test-can)
add_app_subdirectory(${TEST_APPS_DIR}/test-mae3)
add_app_subdirectory(${TEST_APPS_DIR}/test-led-matrix)
add_app_subdirectory(${TEST_APPS_DIR}/test-limit-switch)
add_app_subdirectory(${TEST_APPS_DIR}/test-logger)
add_app_subdirectory(${TEST_APPS_DIR}/test-lookup-table)
add_app_subdirectory(${TEST_APPS_DIR}/test-pid)
add_app_subdirectory(${TEST_APPS_DIR}/test-pwm)
add_app_subdirectory(${TEST_APPS_DIR}/test-pwmin)
add_app_subdirectory(${TEST_APPS_DIR}/test-quadrature64cpr)
add_app_subdirectory(${TEST_APPS_DIR}/test-stress-can)
add_app_subdirectory(${TEST_APPS_DIR}/test-units)
add_app_subdirectory(${TEST_APPS_DIR}/test-watchdog)
add_app_subdirectory(${TEST_APPS_DIR}/tutorial-servo-pot-control)
add_app_subdirectory(${TEST_APPS_DIR}/tutorial-servo-can-control)

# Ensure APP was specified
if ("$ENV{APP}" STREQUAL "")
    message(FATAL_ERROR "APP was not specified")
endif()

# Ensure that a target exists for the specified APP
# https://stackoverflow.com/a/62311397/10173605
function(get_all_targets var)
    set(targets)
    get_all_targets_recursive(targets ${CMAKE_CURRENT_SOURCE_DIR})
    set(${var} ${targets} PARENT_SCOPE)
endfunction()

macro(get_all_targets_recursive targets dir)
    get_property(subdirectories DIRECTORY ${dir} PROPERTY SUBDIRECTORIES)
    foreach (subdir ${subdirectories})
        get_all_targets_recursive(${targets} ${subdir})
    endforeach ()

    get_property(current_targets DIRECTORY ${dir} PROPERTY BUILDSYSTEM_TARGETS)
    list(APPEND ${targets} ${current_targets})
endmacro()

get_all_targets(ALL_BUILDSYSTEM_TARGETS)
if ("$ENV{APP}" IN_LIST ALL_BUILDSYSTEM_TARGETS)
    message(STATUS "$ENV{APP} app is being built")
else ()
    message(FATAL_ERROR "$ENV{APP} app is not an app target found in ${CMAKE_CURRENT_LIST_DIR}/CMakeLists.txt")
endif ()
