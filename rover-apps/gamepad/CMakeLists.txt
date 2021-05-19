add_executable(gamepad)
target_sources(gamepad PRIVATE src/main.cpp)
target_link_libraries(gamepad
        PRIVATE
        AnalogBusIn
        AnalogInputGroup
        DigitalInputGroup
        FrameArbiter
        Logger
        )
mbed_set_post_build(gamepad)
