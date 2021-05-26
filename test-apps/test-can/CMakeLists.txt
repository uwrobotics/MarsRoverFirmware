add_executable(test-can)
target_sources(test-can PRIVATE src/main.cpp)
target_link_libraries(test-can
        PRIVATE
        CANInterface 
        CANBus
        CANMsg
        Logger
        mbed-os
        uwrt-mars-rover-hw-bridge
        )
mbed_set_post_build(test-can)
