add_executable(safety.${TARGET}-board.elf)
target_sources(safety.${TARGET}-board.elf PRIVATE src/main.cpp)
target_set_firmware_properties(safety.${TARGET}-board.elf)
