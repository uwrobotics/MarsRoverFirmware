add_executable(test-fsr.${TARGET}-board.elf)
target_sources(test-fsr.${TARGET}-board.elf PRIVATE src/main.cpp)
target_link_libraries(test-fsr.${TARGET}-board.elf PRIVATE FSR)
target_set_firmware_properties(test-fsr.${TARGET}-board.elf)
