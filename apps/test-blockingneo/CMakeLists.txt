add_executable(test-blockingneo.${TARGET}-board.elf)
target_sources(test-blockingneo.${TARGET}-board.elf PRIVATE src/main.cpp)
target_link_libraries(test-blockingneo.${TARGET}-board.elf PUBLIC Neopixel_Blocking)
target_set_firmware_properties(test-blockingneo.${TARGET}-board.elf)
