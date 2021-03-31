add_executable(test-units.${TARGET}-board.elf)
target_sources(test-units.${TARGET}-board.elf PRIVATE src/main.cpp)
target_link_libraries(test-units.${TARGET}-board.elf PRIVATE units Logger)
target_set_firmware_properties(test-units.${TARGET}-board.elf)