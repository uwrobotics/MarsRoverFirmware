add_executable(test-limit-switch)
target_sources(test-limit-switch PRIVATE src/main.cpp)
target_link_libraries(test-limit-switch PRIVATE LimitSwitch Logger)
mbed_set_post_build(test-limit-switch)
