add_library(safety-board OBJECT)
target_sources(safety-board PRIVATE
        src/PeripheralPins.c
        src/system_clock.c)
target_include_directories(safety-board PUBLIC include)
target_compile_options(safety-board PRIVATE -w) # Disable all warnings from mbed code
target_compile_options(safety-board PRIVATE $<$<COMPILE_LANGUAGE:C,CXX>:-include${CMAKE_SOURCE_DIR}/config/mbed_config.h>)
target_link_libraries(safety-board PUBLIC mbed-os-target-interface)
