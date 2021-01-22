add_executable(test-led-matrix.${TARGET}-board.elf)
target_sources(test-led-matrix.${TARGET}-board.elf PRIVATE src/main.cpp)
target_link_libraries(test-led-matrix.${TARGET}-board.elf PRIVATE LEDMatrix uwrt-mars-rover-hw-bridge)
target_set_firmware_properties(test-led-matrix.${TARGET}-board.elf)
