add_executable(test-servo.${TARGET}-board.elf)
target_sources(test-servo.${TARGET}-board.elf PRIVATE apps/test-servo/main.cpp)
target_set_firmware_properties(test-servo.${TARGET}-board.elf)
