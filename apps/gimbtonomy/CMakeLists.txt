add_executable(gimbtonomy.${TARGET}-board.elf)
target_sources(gimbtonomy.${TARGET}-board.elf PRIVATE src/main.cpp)
target_include_directories(gimbtonomy.${TARGET}-board.elf PUBLIC include)
target_link_libraries(gimbtonomy.${TARGET}-board.elf PRIVATE 
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
        EncoderMAE3
#CAN
        CANBus
        CANMsg
#Sensor
        CurrentSensor
#Other
        Neopixel_Blocking
        Logger
        uwrt-mars-rover-hw-bridge)
target_set_firmware_properties(gimbtonomy.${TARGET}-board.elf)
