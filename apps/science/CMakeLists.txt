add_executable(science.${TARGET}-board.elf)
target_sources(science.${TARGET}-board.elf PRIVATE src/main.cpp)
target_include_directories(science.${TARGET}-board.elf PUBLIC include)
target_link_libraries(science.${TARGET}-board.elf 
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
        CANBus
        CANMsg
#Sensor
        CurrentSensor
        AdafruitSTEMMA
#Other
        uwrt-mars-rover-hw-bridge
        Logger)
target_set_firmware_properties(science.${TARGET}-board.elf)

add_library(ScienceConfig INTERFACE)
target_include_directories(ScienceConfig INTERFACE include)
