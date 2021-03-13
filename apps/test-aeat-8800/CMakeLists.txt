add_executable(test-aeat-8800.${TARGET}-board.elf)
target_sources(test-aeat-8800.${TARGET}-board.elf PRIVATE src/main.cpp)
target_link_libraries(test-aeat-8800.${TARGET}-board.elf PRIVATE Logger)
target_set_firmware_properties(test-aeat-8800.${TARGET}-board.elf)
