add_executable(test-can)
target_sources(test-can PRIVATE src/main.cpp)
target_link_libraries(test-can
        PRIVATE
        CANBus
        CANMsg
        Logger
        mbed-os
        )
mbed_set_post_build(test-can)
