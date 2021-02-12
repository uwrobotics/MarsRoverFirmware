add_library(science-board OBJECT)
target_sources(science-board PRIVATE
        src/PeripheralPins.c
        src/system_clock.c)
target_include_directories(science-board PUBLIC include)
target_compile_definitions(science-board PUBLIC SWO_LOGGING) # Configure logger support
target_compile_options(science-board PRIVATE -w) # Disable all warnings from mbed code
target_compile_options(science-board PRIVATE $<$<COMPILE_LANGUAGE:C,CXX>:-include${CMAKE_SOURCE_DIR}/config/mbed_config.h>)
target_link_libraries(science-board PUBLIC mbed-os-target-interface)
