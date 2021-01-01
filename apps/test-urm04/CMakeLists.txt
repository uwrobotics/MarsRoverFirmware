add_executable(test-urm04.${TARGET}-board.elf)
target_sources(test-urm04.${TARGET}-board.elf PRIVATE src/main.cpp)
target_set_firmware_properties(test-urm04.${TARGET}-board.elf)