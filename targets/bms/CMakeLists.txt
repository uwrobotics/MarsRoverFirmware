add_library(bms-board OBJECT)
target_sources(bms-board PRIVATE
        src/PeripheralPins.c
        src/system_clock.c)
target_include_directories(bms-board PUBLIC include)
target_compile_options(bms-board PRIVATE -w) # Disable all warnings from mbed code
target_compile_options(bms-board PRIVATE $<$<COMPILE_LANGUAGE:C,CXX>:-include${CMAKE_SOURCE_DIR}/config/mbed_config.h>)
target_link_libraries(bms-board PUBLIC mbed-os-target-interface)
