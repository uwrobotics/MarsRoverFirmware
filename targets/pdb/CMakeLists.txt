add_library(pdb-board OBJECT)
target_sources(pdb-board PRIVATE
        src/PeripheralPins.c
        src/system_clock.c)
target_include_directories(pdb-board PUBLIC include)
target_compile_options(pdb-board PRIVATE -w) # Disable all warnings from mbed code
target_compile_options(pdb-board PRIVATE $<$<COMPILE_LANGUAGE:C,CXX>:-include${CMAKE_SOURCE_DIR}/config/mbed_config.h>)
target_link_libraries(pdb-board PUBLIC mbed-os-target-interface)