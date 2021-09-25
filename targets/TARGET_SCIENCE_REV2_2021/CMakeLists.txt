add_library(mbed-science-rev2-2021 INTERFACE)

target_sources(mbed-science-rev2-2021
        INTERFACE
        src/system_clock_override.c
        src/PeripheralPins.c
        )

target_include_directories(mbed-science-rev2-2021
        INTERFACE
        include
        )

target_link_libraries(mbed-science-rev2-2021 INTERFACE mbed-stm32f446xe)
target_compile_options(mbed-science-rev2-2021 INTERFACE -w) # Disable all warnings from mbed target code

