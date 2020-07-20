add_executable(test-enc-abs-pwm.${TARGET}-board.elf)
target_sources(test-enc-abs-pwm.${TARGET}-board.elf PRIVATE src/main.cpp)
target_link_libraries(test-enc-abs-pwm.${TARGET}-board.elf PRIVATE EncoderAbsolute_PWM)
target_set_firmware_properties(test-enc-abs-pwm.${TARGET}-board.elf)
