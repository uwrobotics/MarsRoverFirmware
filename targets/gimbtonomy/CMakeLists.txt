add_library(gimbtonomy-board OBJECT)
target_sources(gimbtonomy-board PRIVATE
        src/PeripheralPins.c
        src/system_clock.c)
target_include_directories(gimbtonomy-board PUBLIC include)
target_compile_definitions(gimbtonomy-board PUBLIC SWO_LOGGING) # Configure logger support
target_compile_options(gimbtonomy-board PRIVATE -w) # Disable all warnings from mbed code
target_compile_options(gimbtonomy-board PRIVATE $<$<COMPILE_LANGUAGE:C,CXX>:-include${CMAKE_SOURCE_DIR}/config/mbed_config.h>)
target_link_libraries(gimbtonomy-board PUBLIC mbed-os-target-interface)
