add_executable(test-can.${TARGET}-board.elf)
target_sources(test-can.${TARGET}-board.elf PRIVATE src/main.cpp)
target_link_libraries(test-can.${TARGET}-board.elf PUBLIC CANMsg)
target_set_firmware_properties(test-can.${TARGET}-board.elf)
