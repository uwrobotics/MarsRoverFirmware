add_executable(science.${TARGET}-board.elf)
target_sources(science.${TARGET}-board.elf PRIVATE src/main.cpp)
target_set_firmware_properties(science.${TARGET}-board.elf)
