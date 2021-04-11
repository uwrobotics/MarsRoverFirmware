add_executable(arm)
target_sources(arm PRIVATE src/main.cpp)
target_include_directories(arm PUBLIC include)
target_link_libraries(arm
        PRIVATE
        #Control
        OpenLoopController
        PositionController
        VelocityController
        CurrentController
        ActuatorControllerManager
        #Motors
        LimServo
        DCMotor
        #Encoders
        EncoderAEAT6012
        EncoderMAE3
        EncoderPololu37D
        #CAN
        CANBus
        CANMsg
        #Sensor
        CurrentSensor
        #Other
        uwrt-mars-rover-hw-bridge
        Logger
        )
mbed_set_post_build(arm)
