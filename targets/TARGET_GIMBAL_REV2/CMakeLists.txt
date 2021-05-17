add_library(mbed-gimbal-rev2 INTERFACE)

target_sources(mbed-gimbal-rev2
        INTERFACE
        src/system_clock_override.c
        src/PeripheralPins.c
        )

target_include_directories(mbed-gimbal-rev2
        INTERFACE
        include
        )

target_link_libraries(mbed-gimbal-rev2 INTERFACE mbed-stm32f446xe)
target_compile_options(mbed-gimbal-rev2 INTERFACE -w) # Disable all warnings from mbed target code

