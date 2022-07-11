add_executable(test-moisture-sensor)
target_sources(test-moisture-sensor PRIVATE src/main.cpp)
target_link_libraries(test-moisture-sensor PRIVATE PollingSensors Logger)
mbed_set_post_build(test-moisture-sensor)
