add_executable(test-moisture.${TARGET}-board.elf)
target_sources(test-moisture.${TARGET}-board.elf PRIVATE src/main.cpp)
target_link_libraries(test-moisture.${TARGET}-board.elf PUBLIC MoistureSensor)
target_set_firmware_properties(test-moisture.${TARGET}-board.elf)
