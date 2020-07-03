add_executable(test-pid.${TARGET}-board.elf)
target_sources(test-pid.${TARGET}-board.elf PRIVATE apps/test-pid/main.cpp)
target_set_firmware_properties(test-pid.${TARGET}-board.elf)
