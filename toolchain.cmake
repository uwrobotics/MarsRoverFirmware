#custom command to use objcopy to create .bin files out of ELF files
#function(make_mbed_firmware INPUT)
#    add_custom_command(TARGET ${INPUT}
#            COMMAND arm-none-eabi-objcopy -O binary ${INPUT} ${INPUT}_${MBED_TARGET}.bin
#            COMMENT "objcopying to make mbed compatible firmware")
#    set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES ${INPUT}_${MBED_TARGET}.bin)
#endfunction(make_mbed_firmware)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_CROSSCOMPILING 1)

set(CMAKE_C_COMPILER arm-none-eabi-gcc CACHE PATH "" FORCE)
set(CMAKE_CXX_COMPILER arm-none-eabi-gcc CACHE PATH "" FORCE)

set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY") #maybe can get rid of this if link flags are right

set(LINKER_SCRIPT_PATH "${CMAKE_SOURCE_DIR}/mbed-os/targets/TARGET_STM/TARGET_STM32F4/TARGET_STM32F446xE/TOOLCHAIN_GCC_ARM/STM32F446XE.ld")
string(APPEND CMAKE_EXE_LINKER_FLAGS " \"-T${LINKER_SCRIPT_PATH}\"")

set(PROCESSOR_FLAGS
        -mcpu=cortex-m4
        -mthumb -mfpu=fpv4-sp-d16
        -mfloat-abi=hard)

set(COMMON_FLAGS
        -include ${CMAKE_SOURCE_DIR}/config/mbed_config.h
        -include ${CMAKE_SOURCE_DIR}/config/rover_config.h
        -include ${CMAKE_SOURCE_DIR}/config/can_config.h

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
        )

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(${PROCESSOR_FLAGS} ${COMMON_FLAGS})
