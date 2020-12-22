add_executable(test-stress-can.${TARGET}-board.elf)
target_sources(test-stress-can.${TARGET}-board.elf PRIVATE src/main.cpp)
target_link_libraries(test-stress-can.${TARGET}-board.elf PRIVATE CANBus CANMsg)
target_set_firmware_properties(test-stress-can.${TARGET}-board.elf)