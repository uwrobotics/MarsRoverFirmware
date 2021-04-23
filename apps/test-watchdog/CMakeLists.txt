add_executable(test-watchdog.${TARGET}-board.elf)
target_sources(test-watchdog.${TARGET}-board.elf PRIVATE src/main.cpp)
target_link_libraries(test-watchdog.${TARGET}-board.elf PRIVATE Watchdog)
target_set_firmware_properties(test-watchdog.${TARGET}-board.elf)
