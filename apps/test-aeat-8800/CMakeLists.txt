add_executable(test-aeat-8800.${TARGET}-board.elf)
target_sources(test-aeat-8800.${TARGET}-board.elf PRIVATE apps/test-aeat-8800/main.cpp)
target_set_firmware_properties(test-aeat-8800.${TARGET}-board.elf)
