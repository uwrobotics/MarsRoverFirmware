add_executable(test-enc-abs-pwm.${TARGET}-board.elf)
target_sources(test-enc-abs-pwm.${TARGET}-board.elf PRIVATE apps/test-enc-abs-pwm/main.cpp)
target_set_firmware_properties(test-enc-abs-pwm.${TARGET}-board.elf)
