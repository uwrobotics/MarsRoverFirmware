add_executable(science_2021)
target_sources(science_2021 PRIVATE ${ROVER_APPS_DIR}/common/src/main.cpp)
target_include_directories(science_2021 PUBLIC include ${ROVER_APPS_DIR}/common/include)
target_link_libraries(science_2021
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
        AdafruitSTEMMA
        #common-modules
        WatchdogModule
        #Other
        uwrt-mars-rover-hw-bridge
        Logger
        mbed-events
        )
mbed_set_post_build(science_2021)

add_library(ScienceConfig INTERFACE)
target_include_directories(ScienceConfig INTERFACE include)
