add_executable(gimbtonomy.${TARGET}-board.elf)
target_sources(gimbtonomy.${TARGET}-board.elf PRIVATE apps/gimbtonomy/main.cpp)
target_set_firmware_properties(gimbtonomy.${TARGET}-board.elf)
