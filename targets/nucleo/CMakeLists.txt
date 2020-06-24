add_library(nucleo-board OBJECT)
target_sources(nucleo-board PRIVATE
        src/PeripheralPins.c
        src/system_clock.c)
target_include_directories(nucleo-board PUBLIC include)
target_compile_options(nucleo-board PRIVATE -w) # Disable all warnings from mbed code
target_link_libraries(nucleo-board PUBLIC mbed-os-target-interface)
