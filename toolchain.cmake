set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_CROSSCOMPILING 1)
enable_language(ASM)

set(CMAKE_ASM_COMPILER arm-none-eabi-gcc CACHE PATH "" FORCE)
set(CMAKE_C_COMPILER arm-none-eabi-gcc CACHE PATH "" FORCE)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++ CACHE PATH "" FORCE)
set(CMAKE_AS arm-none-eabi-as CACHE PATH "" FORCE)
set(CMAKE_AR arm-none-eabi-ar CACHE PATH "" FORCE)
set(CMAKE_CPP arm-none-eabi-cpp CACHE PATH "" FORCE)
set(CMAKE_LINKER arm-none-eabi-ld CACHE PATH "" FORCE)
set(CMAKE_NM arm-none-eabi-nm CACHE PATH "" FORCE)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy CACHE PATH "" FORCE)
set(CMAKE_OBJDUMP arm-none-eabi-objdump CACHE PATH "" FORCE)
set(CMAKE_READELF arm-none-eabi-readelf CACHE PATH "" FORCE)
set(CMAKE_SIZE arm-none-eabi-size CACHE PATH "" FORCE)
set(CMAKE_STRINGS arm-none-eabi-strings CACHE PATH "" FORCE)
set(CMAKE_STRIP arm-none-eabi-strip CACHE PATH "" FORCE)

set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY") #maybe can get rid of this if link flags are right

# Flags
set(PROCESSOR_FLAGS
        -mcpu=cortex-m4
        -mthumb
        -mfpu=fpv4-sp-d16
        -mfloat-abi=hard)
set(MEMORY_DEFINITIONS
        MBED_ROM_START=0x8000000
        MBED_ROM_SIZE=0x80000
        MBED_RAM_START=0x20000000
        MBED_RAM_SIZE=0x20000
        MBED_BOOT_STACK_SIZE=1024
        )
set(MEMORY_DEFINITIONS_FOR_LINKER ${MEMORY_DEFINITIONS})
list(TRANSFORM MEMORY_DEFINITIONS_FOR_LINKER PREPEND -D)
# Linker Script and Flags
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
    string(REPLACE ".elf" "" BASE-TARGET-NAME TARGET-NAME)
    add_custom_command(TARGET ${TARGET-NAME}
            POST_BUILD
            COMMAND ${CMAKE_OBJCOPY} -O binary ${INPUT} ${BASE-TARGET-NAME}.bin
            COMMENT "Generating bin from elf")
    set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES ${BASE-TARGET-NAME}.bin)
endfunction()

# link against mbed and setup linker script
function(target_set_firmware_properties TARGET-NAME)
    target_link_libraries(${TARGET-NAME} PUBLIC mbed-os)
    target_precompile_headers(${TARGET-NAME} REUSE_FROM mbed-os)

    set_target_properties(${TARGET-NAME} PROPERTIES LINK_DEPENDS ${LINKER_SCRIPT_OUTPUT_PATH})
    add_dependencies(${TARGET-NAME} linker-script)

    target_bin_from_elf(${TARGET-NAME})
endfunction()
