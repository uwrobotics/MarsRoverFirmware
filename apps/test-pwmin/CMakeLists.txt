add_executable(test-pwmin.${TARGET}-board.elf)
target_sources(test-pwmin.${TARGET}-board.elf PRIVATE src/main.cpp)
target_link_libraries(test-pwmin.${TARGET}-board.elf PRIVATE PwmIn)
target_set_firmware_properties(test-pwmin.${TARGET}-board.elf)
