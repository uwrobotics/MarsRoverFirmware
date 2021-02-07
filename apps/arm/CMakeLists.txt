add_executable(arm.${TARGET}-board.elf)
target_sources(arm.${TARGET}-board.elf PRIVATE src/main.cpp)
target_link_libraries(arm.${TARGET}-board.elf
        PRIVATE
        ActuatorControllerManager
        ActuatorController
        Actuator
        Encoder
        CurrentSensor
        PID
        CANBus
        CANMsg
        uwrt-mars-rover-hw-bridge
        Config
        )
target_set_firmware_properties(arm.${TARGET}-board.elf)

add_library(Config INTERFACE)
target_include_directories(Config INTERFACE include ActuatorControllerManager)
