add_executable(test-blinky)
target_sources(test-blinky PRIVATE src/main.cpp)
target_link_libraries(test-blinky
        PRIVATE
        Logger
        mbed-os
        )
mbed_set_post_build(test-blinky)
