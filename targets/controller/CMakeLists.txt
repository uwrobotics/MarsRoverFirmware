add_library(controller-board OBJECT)
target_sources(controller-board PRIVATE
        src/PeripheralPins.c
        src/system_clock.c)
target_include_directories(controller-board PUBLIC include)
target_compile_options(controller-board PRIVATE -w) # Disable all warnings from mbed code
target_compile_options(controller-board PRIVATE $<$<COMPILE_LANGUAGE:C,CXX>:-include${CMAKE_SOURCE_DIR}/config/mbed_config.h>)
target_link_libraries(controller-board PUBLIC mbed-os-target-interface)
