add_executable(test-limit-switch.${TARGET}-board.elf)
target_sources(test-limit-switch.${TARGET}-board.elf PRIVATE src/main.cpp)
target_set_firmware_properties(test-limit-switch.${TARGET}-board.elf)