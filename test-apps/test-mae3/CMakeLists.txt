add_executable(test-mae3)
target_sources(test-mae3 PRIVATE src/main.cpp)
target_link_libraries(test-mae3
        PRIVATE
        EncoderMAE3
        Logger
        PwmIn
        )
mbed_set_post_build(test-mae3)
