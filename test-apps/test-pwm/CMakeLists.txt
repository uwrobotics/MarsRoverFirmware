add_executable(test-pwm)
target_sources(test-pwm PRIVATE src/main.cpp)
target_link_libraries(test-pwm PRIVATE mbed-os)
mbed_set_post_build(test-pwm)
