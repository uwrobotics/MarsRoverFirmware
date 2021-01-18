add_executable(test-pwm.${TARGET}-board.elf)
target_sources(test-pwm.${TARGET}-board.elf PRIVATE src/main.cpp)
target_set_firmware_properties(test-pwm.${TARGET}-board.elf)

