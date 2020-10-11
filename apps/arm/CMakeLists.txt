add_executable(arm.${TARGET}-board.elf)
target_sources(arm.${TARGET}-board.elf PRIVATE src/main.cpp)
target_link_libraries(arm.${TARGET}-board.elf
        PUBLIC
        PID
        PRIVATE
        ClawController
        CANBuffer
        CANBus
        DifferentialWristController
        EncoderRelative_Quadrature
        LimServo
        Motor
        uwrt-mars-rover-hw-bridge
        )
target_set_firmware_properties(arm.${TARGET}-board.elf)

add_library(DifferentialWristController STATIC)
target_sources(DifferentialWristController PRIVATE src/DifferentialWristController.cpp)
target_include_directories(DifferentialWristController PUBLIC include)
target_link_libraries(DifferentialWristController PRIVATE ActuatorController Encoder PID Motor)
target_set_mbed_dependency(DifferentialWristController)

add_library(ClawController STATIC)
target_sources(ClawController PRIVATE src/ClawController.cpp)
target_include_directories(ClawController PUBLIC include)
target_link_libraries(ClawController PRIVATE ActuatorController Encoder PID LimServo uwrt-mars-rover-hw-bridge)
target_set_mbed_dependency(ClawController)
