add_executable(test-logger.${TARGET}-board.elf)
target_sources(test-logger.${TARGET}-board.elf PRIVATE src/main.cpp)
target_link_libraries(test-logger.${TARGET}-board.elf PRIVATE Logger)
target_set_firmware_properties(test-logger.${TARGET}-board.elf)
