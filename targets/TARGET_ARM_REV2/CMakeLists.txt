add_library(mbed-arm-rev2 INTERFACE)

target_sources(mbed-arm-rev2
        INTERFACE
        src/system_clock_override.c
        src/PeripheralPins.c
        )

target_include_directories(mbed-arm-rev2
        INTERFACE
        include
        )

target_link_libraries(mbed-arm-rev2 INTERFACE mbed-stm32f446xe)
target_compile_options(mbed-arm-rev2 INTERFACE -w) # Disable all warnings from mbed target code

