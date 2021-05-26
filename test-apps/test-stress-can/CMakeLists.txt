add_executable(test-stress-can)
target_sources(test-stress-can PRIVATE src/main.cpp)
target_include_directories(test-stress-can PUBLIC include)
target_link_libraries(test-stress-can
    PRIVATE 
    CANInterface 
    CANMsg     
    uwrt-mars-rover-hw-bridge
    mbed-os
    Logger
)
mbed_set_post_build(test-stress-can)
