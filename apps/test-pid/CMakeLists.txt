add_executable(test-pid.${TARGET}-board.elf)
target_sources(test-pid.${TARGET}-board.elf PRIVATE src/main.cpp)
target_link_libraries(test-pid.${TARGET}-board.elf PRIVATE PID)
target_set_firmware_properties(test-pid.${TARGET}-board.elf)
