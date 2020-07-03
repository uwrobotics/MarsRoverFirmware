add_executable(test-moisture.${TARGET}-board.elf)
target_sources(test-moisture.${TARGET}-board.elf PRIVATE apps/test-moisture/main.cpp)
target_set_firmware_properties(test-moisture.${TARGET}-board.elf)
