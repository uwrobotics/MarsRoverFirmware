add_executable(test-temp-RH)
target_sources(test-temp-RH PRIVATE src/main.cpp)
target_link_libraries(test-temp-RH PRIVATE DHT Logger)
mbed_set_post_build(test-temp-RH)
