add_library(mbed-gamepad-rev1-2021 INTERFACE)

target_sources(mbed-gamepad-rev1-2021
        INTERFACE
        src/system_clock_override.c
        src/PeripheralPins.c
        )

target_include_directories(mbed-gamepad-rev1-2021
        INTERFACE
        include
        )

target_link_libraries(mbed-gamepad-rev1-2021 INTERFACE mbed-stm32f446xe)
target_compile_options(mbed-gamepad-rev1-2021 INTERFACE -w) # Disable all warnings from mbed target code

