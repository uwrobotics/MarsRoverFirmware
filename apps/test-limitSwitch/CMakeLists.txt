add_executable(test-limitSwitch.${TARGET}-board.elf)
target_sources(test-limitSwitch.${TARGET}-board.elf PRIVATE src/main.cpp)
target_set_firmware_properties(test-limitSwitch.${TARGET}-board.elf)