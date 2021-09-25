add_executable(gamepad_2021)
target_sources(gamepad_2021 PRIVATE src/main.cpp)
target_link_libraries(gamepad_2021
        PRIVATE
        AnalogBusIn
        AnalogInputGroup
        DigitalInputGroup
        FrameArbiter
        Logger
        )
mbed_set_post_build(gamepad_2021)
