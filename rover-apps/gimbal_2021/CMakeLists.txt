<<<<<<< HEAD:rover-apps/gimbal_2021/CMakeLists.txt
add_executable(gimbal_2021)
target_sources(gimbal_2021 PRIVATE src/main.cpp)
target_include_directories(gimbal_2021 PUBLIC include)
target_link_libraries(gimbal_2021
=======
add_executable(gimbal)
target_sources(gimbal PRIVATE ../common/src/main.cpp)
target_include_directories(gimbal PUBLIC include ../common/include)
target_link_libraries(gimbal
>>>>>>> Create modules skeleton for new app structure (#382):rover-apps/gimbal/CMakeLists.txt
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
mbed_set_post_build(gimbal_2021)
