add_executable(arm.${TARGET}-board.elf)
target_sources(arm.${TARGET}-board.elf PRIVATE src/main.cpp)
target_link_libraries(arm.${TARGET}-board.elf
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
        Config
        )
target_set_firmware_properties(arm.${TARGET}-board.elf)

add_library(Config INTERFACE)
target_include_directories(Config INTERFACE include ActuatorControllerManager)
