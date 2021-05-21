add_executable(test-led-matrix)
target_sources(test-led-matrix PRIVATE src/main.cpp)
target_link_libraries(test-led-matrix 
        PRIVATE 
        LEDMatrix 
        uwrt-mars-rover-hw-bridge
        mbed-os
        )
mbed_set_post_build(test-led-matrix)
