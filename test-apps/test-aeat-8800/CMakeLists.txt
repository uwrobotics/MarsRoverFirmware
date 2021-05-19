add_executable(test-aeat-8800)
target_sources(test-aeat-8800 PRIVATE src/main.cpp)
target_link_libraries(test-aeat-8800 PRIVATE Logger)
mbed_set_post_build(test-aeat-8800)
