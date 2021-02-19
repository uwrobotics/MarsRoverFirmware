add_executable(test-pololu370-encoder.${TARGET}-board.elf)
target_sources(test-pololu370-encoder.${TARGET}-board.elf PRIVATE src/main.cpp)
target_link_libraries(test-pololu370-encoder.${TARGET}-board.elf PRIVATE EncoderPololu37D QEI Logger)
target_set_firmware_properties(test-pololu370-encoder.${TARGET}-board.elf)
