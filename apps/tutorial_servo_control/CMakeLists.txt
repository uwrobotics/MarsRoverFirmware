add_executable(tutorial_servo_controller.${TARGET}-board.elf)
target_sources(tutorial_servo_controller.${TARGET}-board.elf PRIVATE src/main.cpp)
target_set_firmware_properties(tutorial_servo_controller.${TARGET}-board.elf)
