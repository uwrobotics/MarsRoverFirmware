add_library(localization-board OBJECT)
target_sources(localization-board PRIVATE
        src/PeripheralPins.c
        src/system_clock.c)
target_include_directories(localization-board PUBLIC include)
target_compile_options(localization-board PRIVATE -w) # Disable all warnings from mbed code
target_compile_options(localization-board PRIVATE $<$<COMPILE_LANGUAGE:C,CXX>:-include${CMAKE_SOURCE_DIR}/config/mbed_config.h>)
target_link_libraries(localization-board PUBLIC mbed-os-target-interface)
