add_executable(test-adafruitSTEMMA)
target_sources(test-adafruitSTEMMA PRIVATE src/main.cpp)
target_include_directories(test-adafruitSTEMMA PUBLIC include)
target_link_libraries(test-adafruitSTEMMA
        PRIVATE
        AdafruitSTEMMA
        Logger
        uwrt-mars-rover-hw-bridge
        )
mbed_set_post_build(test-adafruitSTEMMA)
