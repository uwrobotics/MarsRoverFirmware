add_executable(test-units)
target_sources(test-units PRIVATE src/main.cpp)
target_link_libraries(test-units PRIVATE Units Logger)
mbed_set_post_build(test-units)
