add_executable(test-mae3.${TARGET}-board.elf)
target_sources(test-mae3.${TARGET}-board.elf PRIVATE src/main.cpp)
target_link_libraries(test-mae3.${TARGET}-board.elf PRIVATE EncoderMAE3 Logger PwmIn)
target_set_firmware_properties(test-mae3.${TARGET}-board.elf)
