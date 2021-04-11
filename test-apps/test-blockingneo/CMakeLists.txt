add_executable(test-blockingneo)
target_sources(test-blockingneo PRIVATE src/main.cpp)
target_link_libraries(test-blockingneo
        PRIVATE
        mbed-os
        Neopixel_Blocking
        uwrt-mars-rover-hw-bridge
        )
mbed_set_post_build(test-blockingneo)
