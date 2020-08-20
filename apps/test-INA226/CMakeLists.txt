add_executable(test-ina226.${TARGET}-board.elf)
target_sources(test-ina226.${TARGET}-board.elf PRIVATE src/main.cpp)
target_link_libraries(test-ina226.${TARGET}-board.elf PRIVATE CANMsg uwrt-mars-rover-hw-bridge)
target_set_firmware_properties(test-ina226.${TARGET}-board.elf)

