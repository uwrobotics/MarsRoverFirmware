add_library(LEDMatrix STATIC)
target_sources(LEDMatrix PRIVATE src/LEDMatrix.cpp)
target_include_directories(LEDMatrix PUBLIC include)
target_link_libraries(LEDMatrix
        PRIVATE
        uwrt-mars-rover-hw-bridge
        )
target_set_mbed_dependency(LEDMatrix)
