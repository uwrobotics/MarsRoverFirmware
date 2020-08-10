add_executable(test-actuator-controller.${TARGET}-board.elf)
target_sources(test-actuator-controller.${TARGET}-board.elf PRIVATE src/main.cpp)
target_link_libraries(test-actuator-controller.${TARGET}-board.elf PRIVATE ActuatorController EncoderRelative_Quadrature)
target_set_firmware_properties(test-actuator-controller.${TARGET}-board.elf)
