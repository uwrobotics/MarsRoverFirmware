add_executable(test-aeat-6012)
target_sources(test-aeat-6012 PRIVATE src/main.cpp)
target_link_libraries(test-aeat-6012
        PRIVATE
        EncoderAEAT6012
        Logger
        )
mbed_set_post_build(test-aeat-6012)
