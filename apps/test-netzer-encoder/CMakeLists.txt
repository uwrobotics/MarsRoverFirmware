add_executable(test-netzer-encoder.${TARGET}-board.elf)
target_sources(test-netzer-encoder.${TARGET}-board.elf PRIVATE src/main.cpp)
target_set_firmware_properties(test-netzer-encoder.${TARGET}-board.elf)
