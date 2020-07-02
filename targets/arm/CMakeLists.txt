add_library(arm-board OBJECT)
target_sources(arm-board PRIVATE
        src/PeripheralPins.c
        src/system_clock.c)
target_include_directories(arm-board PUBLIC include)
target_compile_options(arm-board PRIVATE -w) # Disable all warnings from mbed code
target_link_libraries(arm-board PUBLIC mbed-os-target-interface)
