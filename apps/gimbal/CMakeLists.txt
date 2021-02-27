add_executable(gimbal.${TARGET}-board.elf)
target_sources(gimbal.${TARGET}-board.elf PRIVATE src/main.cpp)
target_include_directories(gimbal.${TARGET}-board.elf PUBLIC include)
target_link_libraries(gimbal.${TARGET}-board.elf PRIVATE 
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
        CANBus
        CANMsg
#Sensor
        CurrentSensor
#Other
        Logger 
        uwrt-mars-rover-hw-bridge)
target_set_firmware_properties(gimbal.${TARGET}-board.elf)
