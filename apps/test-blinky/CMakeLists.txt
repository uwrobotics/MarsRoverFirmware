add_executable(test-blinky.${TARGET}-board.elf)
target_sources(test-blinky.${TARGET}-board.elf PRIVATE src/main.cpp)
target_link_libraries(test-blinky.${TARGET}-board.elf PRIVATE Logger)
target_set_firmware_properties(test-blinky.${TARGET}-board.elf)
