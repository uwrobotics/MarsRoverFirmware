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

if (NOT DEFINED APP)
    message(FATAL_ERROR "APP cache variable not set")
else ()
    # TODO(wmmc88): check if APP matches folder in apps and TARGET matches folder in targets
endif ()

if (NOT DEFINED TARGET)
    message(FATAL_ERROR "TARGET cache variable not set")
endif ()
message(STATUS "Configuring and generating build for ${APP} app on ${TARGET} target")

# TODO(wmmc88): check if its in supported configuration list and warn if not in list

add_subdirectory(targets/${TARGET})

add_executable(test-blinky.elf)
target_sources(test-blinky.elf PRIVATE apps/test-blinky/main.cpp)
target_link_libraries(test-blinky.elf PUBLIC ${TARGET}-board mbed-os) # TODO(wmmc88): remove target-board dep
target_precompile_headers(test-blinky.elf REUSE_FROM mbed-os)
set_target_properties(test-blinky.elf PROPERTIES LINK_DEPENDS ${LINKER_SCRIPT_OUTPUT_PATH})
add_dependencies(test-blinky.elf linker-script)

