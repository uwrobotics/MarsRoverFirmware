add_executable(test-actuator-controller)
target_sources(test-actuator-controller PRIVATE src/main.cpp)
target_link_libraries(test-actuator-controller PRIVATE mbed-os)
mbed_set_post_build(test-actuator-controller)
