add_executable(test-logger)
target_sources(test-logger PRIVATE src/main.cpp)
target_link_libraries(test-logger PRIVATE Logger)
mbed_set_post_build(test-logger)
