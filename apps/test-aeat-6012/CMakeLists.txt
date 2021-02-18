add_executable(test-aeat-6012.${TARGET}-board.elf)
target_sources(test-aeat-6012.${TARGET}-board.elf PRIVATE src/main.cpp)
target_link_libraries(test-aeat-6012.${TARGET}-board.elf PRIVATE EncoderAEAT6012 Logger)
target_set_firmware_properties(test-aeat-6012.${TARGET}-board.elf)
