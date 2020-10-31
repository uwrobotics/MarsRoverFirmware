add_library(gamepad-board OBJECT)
target_sources(gamepad-board PRIVATE
        src/PeripheralPins.c
        src/system_clock.c)
target_include_directories(gamepad-board PUBLIC include)
target_compile_options(gamepad-board PRIVATE -w) # Disable all warnings from mbed code
target_compile_options(gamepad-board PRIVATE $<$<COMPILE_LANGUAGE:C,CXX>:-include${CMAKE_SOURCE_DIR}/config/mbed_config.h>)
target_link_libraries(gamepad-board PUBLIC mbed-os-target-interface)
