add_executable(test-CO2-sensor)
target_sources(test-CO2-sensor PRIVATE src/main.cpp)
target_link_libraries(test-CO2-sensor PRIVATE PollingSensors Logger)
mbed_set_post_build(test-CO2-sensor)
