add_executable(test-watchdog)
target_sources(test-watchdog PRIVATE src/main.cpp)
target_link_libraries(test-watchdog PRIVATE PwmIn WatchdogWrapper)
mbed_set_post_build(test-watchdog)
