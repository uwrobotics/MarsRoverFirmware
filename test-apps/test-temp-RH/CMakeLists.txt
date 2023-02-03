add_executable(test-temp-RH)
target_sources(test-temp-RH PRIVATE src/main.cpp)
target_include_directories(test-temp-RH PUBLIC include)
target_link_libraries(test-temp-RH 
        PRIVATE 
        DHT22 
        Logger
        uwrt-mars-rover-hw-bridge
        )
mbed_set_post_build(test-temp-RH)
