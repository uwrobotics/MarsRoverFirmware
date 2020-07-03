add_executable(science.${TARGET}-board.elf)
target_sources(science.${TARGET}-board.elf PRIVATE apps/science/main.cpp)
target_set_firmware_properties(science.${TARGET}-board.elf)
