add_executable(test-pid-tuning-api.${TARGET}-board.elf)
target_sources(test-pid-tuning-api.${TARGET}-board.elf PRIVATE src/main.cpp)
target_link_libraries(test-pid-tuning-api.${TARGET}-board.elf
        PRIVATE
        PID
        CANBuffer
        EncoderRelative_Quadrature
        Motor
        testPIDTuningApi
        uwrt-mars-rover-hw-bridge
        )
target_set_firmware_properties(test-pid-tuning-api.${TARGET}-board.elf)

add_library(testPIDTuningApi STATIC)
target_sources(testPIDTuningApi PRIVATE src/testPIDTuningApi.cpp)
target_include_directories(testPIDTuningApi PUBLIC include)
target_link_libraries(testPIDTuningApi PRIVATE ActuatorController CANMsg Encoder PID Motor uwrt-mars-rover-hw-bridge)
target_set_mbed_dependency(testPIDTuningApi)
