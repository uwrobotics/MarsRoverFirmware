add_executable(gimbal)
target_sources(gimbal PRIVATE src/main.cpp)
target_include_directories(gimbal PUBLIC include)
target_link_libraries(gimbal
        PRIVATE
        #Control
        OpenLoopController
        PositionController
        VelocityController
        CurrentController
        ActuatorControllerManager
        #Motors
        LimServo
        ContServo
        #Encoders
        EncoderAEAT6012
        #CAN
        CANInterface
        CANMsg
        #Sensor
        CurrentSensor
        #Other
        Logger
        uwrt-mars-rover-hw-bridge
        mbed-events
        )
mbed_set_post_build(gimbal)
