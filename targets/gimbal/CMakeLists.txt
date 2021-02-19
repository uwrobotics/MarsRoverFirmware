add_library(gimbal-board OBJECT)
target_sources(gimbal-board PRIVATE
        src/PeripheralPins.c
        src/system_clock.c)
target_include_directories(gimbal-board PUBLIC include)
target_compile_options(gimbal-board PRIVATE -w) # Disable all warnings from mbed code
target_compile_options(gimbal-board PRIVATE $<$<COMPILE_LANGUAGE:C,CXX>:-include${CMAKE_SOURCE_DIR}/config/mbed_config.h>)
target_link_libraries(gimbal-board PUBLIC mbed-os-target-interface)
