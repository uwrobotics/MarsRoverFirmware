add_executable(arm.${TARGET}-board.elf)
target_sources(arm.${TARGET}-board.elf PRIVATE src/main.cpp)
target_link_libraries(arm.${TARGET}-board.elf
        PUBLIC
        PID
        PRIVATE
        ClawController
        CANBuffer
        DifferentialWristController
        EncoderRelative_Quadrature
        LimServo
        Motor
        uwrt-mars-rover-hw-bridge
        )
target_set_firmware_properties(arm.${TARGET}-board.elf)

add_library(testPIDTuningApi STATIC)
target_sources(testPIDTuningApi PRIVATE src/testPIDTuningApi.cpp)
target_include_directories(testPIDTuningApi PUBLIC include)
target_link_libraries(testPIDTuningApi PRIVATE ActuatorController Encoder PID Motor)
target_set_mbed_dependency(testPIDTuningApi)
