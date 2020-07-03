add_executable(test-can.${TARGET}-board.elf)
target_sources(test-can.${TARGET}-board.elf PRIVATE apps/test-can/main.cpp)
target_set_firmware_properties(test-can.${TARGET}-board.elf)
