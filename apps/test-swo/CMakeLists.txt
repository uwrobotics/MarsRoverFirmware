add_executable(test-swo.${TARGET}-board.elf)
target_sources(test-swo.${TARGET}-board.elf PRIVATE src/main.cpp)
target_link_libraries(test-swo.${TARGET}-board.elf PRIVATE SWO)
target_set_firmware_properties(test-swo.${TARGET}-board.elf)
