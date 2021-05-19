add_executable(test-pwmin)
target_sources(test-pwmin PRIVATE src/main.cpp)
target_link_libraries(test-pwmin PRIVATE PwmIn Logger)
mbed_set_post_build(test-pwmin)
