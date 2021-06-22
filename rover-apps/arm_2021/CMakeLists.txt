<<<<<<< HEAD:rover-apps/arm_2021/CMakeLists.txt
add_executable(arm_2021)
target_sources(arm_2021 PRIVATE src/main.cpp)
target_include_directories(arm_2021 PUBLIC include)
target_link_libraries(arm_2021
=======
add_executable(arm)
target_sources(arm PRIVATE ../common/src/main.cpp)
target_include_directories(arm PUBLIC include ../common/include)
target_link_libraries(arm
>>>>>>> Create modules skeleton for new app structure (#382):rover-apps/arm/CMakeLists.txt
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
        Quadrature64CPR
        #CAN
        CANInterface
        CANMsg
        #Sensor
        CurrentSensor
        #Other
        uwrt-mars-rover-hw-bridge
        Logger
        #Mbed
        mbed-os
        mbed-events
        )
mbed_set_post_build(arm_2021)
