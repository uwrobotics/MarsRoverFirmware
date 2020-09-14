add_executable(tutorial_servo_control.${TARGET}-board.elf)
target_sources(tutorial_servo_control.${TARGET}-board.elf PRIVATE src/main.cpp)
target_set_firmware_properties(tutorial_Servo_control.${TARGET}-board.elf)
