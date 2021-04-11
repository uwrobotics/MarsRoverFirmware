add_library(mbed-pdb-rev1 INTERFACE)

target_sources(mbed-pdb-rev1
        INTERFACE
        src/system_clock_override.c
        src/PeripheralPins.c
        )

target_include_directories(mbed-pdb-rev1
        INTERFACE
        include
        )

target_link_libraries(mbed-pdb-rev1 INTERFACE mbed-stm32f446xe)
target_compile_options(mbed-pdb-rev1 INTERFACE -w) # Disable all warnings from mbed target code

