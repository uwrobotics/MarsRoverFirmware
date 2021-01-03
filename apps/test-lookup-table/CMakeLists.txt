add_executable(test-lookup-table.${TARGET}-board.elf)
target_sources(test-lookup-table.${TARGET}-board.elf PRIVATE src/main.cpp)
target_link_libraries(test-lookup-table.${TARGET}-board.elf PRIVATE LookupTable CANMsg)
target_set_firmware_properties(test-lookup-table.${TARGET}-board.elf)
