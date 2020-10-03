cmake_minimum_required(VERSION 3.15.7)

project(MarsRoverFirmware)

set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

enable_language(C ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Processor and Memory Settings
set(PROCESSOR_FLAGS
        -mcpu=cortex-m4
        -mthumb
        -mfpu=fpv4-sp-d16
        -mfloat-abi=hard
        )
set(MEMORY_DEFINITIONS
        MBED_ROM_START=0x8000000
        MBED_ROM_SIZE=0x80000
        MBED_RAM_START=0x20000000
        MBED_RAM_SIZE=0x20000
        MBED_BOOT_STACK_SIZE=1024
        )
set(MEMORY_DEFINITIONS_FOR_LINKER ${MEMORY_DEFINITIONS})
list(TRANSFORM MEMORY_DEFINITIONS_FOR_LINKER PREPEND -D)

# Linker Script, Flags, Targets
set(LINKER_FLAGS
        -specs=nosys.specs
        LINKER:--gc-sections
        LINKER:--wrap=main
        LINKER:--wrap=__malloc_r
        LINKER:--wrap=__free_r
        LINKER:--wrap=__realloc_r
        LINKER:--wrap=__memalign_r
        LINKER:--wrap=__calloc_r
        LINKER:--wrap=exit
        LINKER:--wrap=atexit
        LINKER:-n
        )
set(LINKER_SYS_LIBS
        LINKER:--start-group
        -lstdc++
        -lsupc++
        -lm
        -lc
        -lgcc
        -lnosys
        LINKER:--end-group
        )
set(LINKER_SCRIPT_SOURCE_PATH "${CMAKE_SOURCE_DIR}/mbed-os/targets/TARGET_STM/TARGET_STM32F4/TARGET_STM32F446xE/TOOLCHAIN_GCC_ARM/STM32F446XE.ld")
set(LINKER_SCRIPT_OUTPUT_PATH "${CMAKE_CURRENT_BINARY_DIR}/STM32F446XE.out.ld")
add_custom_command(OUTPUT ${LINKER_SCRIPT_OUTPUT_PATH}
        COMMAND ${CMAKE_CPP} -E -P ${PROCESSOR_FLAGS} ${MEMORY_DEFINITIONS_FOR_LINKER} ${LINKER_SCRIPT_SOURCE_PATH} -o ${LINKER_SCRIPT_OUTPUT_PATH}
        MAIN_DEPENDENCY ${LINKER_SCRIPT_SOURCE_PATH}
        COMMENT "Preprocessing linker script"
        )
add_custom_target(linker-script DEPENDS ${LINKER_SCRIPT_OUTPUT_PATH})
add_link_options(-T${LINKER_SCRIPT_OUTPUT_PATH} ${LINKER_FLAGS} ${PROCESSOR_FLAGS} ${MEMORY_DEFINITIONS_FOR_LINKER} ${LINKER_SYS_LIBS})

# use objcopy to create .bin files out of ELF files
function(target_bin_from_elf TARGET-NAME)
    string(REPLACE ".elf" "" BASE-TARGET-NAME ${TARGET-NAME})
    add_custom_command(TARGET ${TARGET-NAME}
            POST_BUILD
            COMMAND ${CMAKE_OBJCOPY} -O binary ${TARGET-NAME} ${BASE-TARGET-NAME}.bin
            COMMENT "Generating bin from elf")
    set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES ${BASE-TARGET-NAME}.bin)
endfunction()

# link against mbed and precompile headers
function(target_set_mbed_dependency TARGET-NAME)
    target_link_libraries(${TARGET-NAME} PRIVATE mbed-os)
    target_precompile_headers(${TARGET-NAME} REUSE_FROM mbed-os)
endfunction()

# link against mbed and setup linker script
function(target_set_firmware_properties TARGET-NAME)
    target_set_mbed_dependency(${TARGET-NAME})

    set_target_properties(${TARGET-NAME} PROPERTIES LINK_DEPENDS ${LINKER_SCRIPT_OUTPUT_PATH})
    add_dependencies(${TARGET-NAME} linker-script)

    target_bin_from_elf(${TARGET-NAME})
endfunction()

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
        )
add_compile_options(${PROCESSOR_FLAGS} ${COMPILE_FLAGS})

# Include mbed-os
include(mbed-os.cmake)

# Include library CMakeLists
add_subdirectory(lib/can)
add_subdirectory(lib/gamepad)
add_subdirectory(lib/controllers)
add_subdirectory(lib/neopixel)
add_subdirectory(lib/pid)
add_subdirectory(lib/sensors)
add_subdirectory(lib/servo)

if (NOT DEFINED APP)
    message(FATAL_ERROR "APP variable not set in CACHE. Please invoke CMake with \"-DAPP=<app-name>\"")
elseif (NOT EXISTS "${CMAKE_SOURCE_DIR}/apps/${APP}")
    message(FATAL_ERROR "${APP} app does not exist in the apps folder")
endif ()
if (NOT DEFINED TARGET)
    message(FATAL_ERROR "TARGET variable not set in CACHE. Please invoke CMake with \"-DTARGET=<target-name>\"")
elseif (NOT EXISTS "${CMAKE_SOURCE_DIR}/targets/${TARGET}")
    message(FATAL_ERROR "${TARGET} target does not exist in the targets folder")
endif ()
message(STATUS "Configuring and generating build for ${APP} app on ${TARGET} target")

# Add CMakeList for target-specific files
add_subdirectory(targets/${TARGET})

# Add HW Bridge
add_subdirectory(uwrt-mars-rover-hw-bridge)

# Include Rover App CMakeLists
add_subdirectory(apps/arm)
add_subdirectory(apps/gamepad)
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
add_subdirectory(apps/test-moisture)
add_subdirectory(apps/test-pid)
add_subdirectory(apps/test-pwm)
add_subdirectory(apps/test-pwmin)
