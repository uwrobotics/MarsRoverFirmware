add_executable(test-actuator-controller.${TARGET}-board.elf)
target_sources(test-actuator-controller.${TARGET}-board.elf PRIVATE apps/test-actuator-controller/main.cpp)
target_set_firmware_properties(test-actuator-controller.${TARGET}-board.elf)
