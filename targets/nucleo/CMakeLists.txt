add_library(nucleo-board OBJECT)
target_sources(nucleo-board PRIVATE
        src/PeripheralPins.c
        src/system_clock.c)
target_include_directories(nucleo-board PUBLIC include)
target_compile_definitions(nucleo-board PUBLIC UART_LOGGING) # Configure logger support
target_compile_options(nucleo-board PRIVATE -w) # Disable all warnings from mbed code
target_compile_options(nucleo-board PRIVATE $<$<COMPILE_LANGUAGE:C,CXX>:-include${CMAKE_SOURCE_DIR}/config/mbed_config.h>)
target_link_libraries(nucleo-board PUBLIC mbed-os-target-interface)
