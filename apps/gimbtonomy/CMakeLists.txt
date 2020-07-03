add_executable(gimbtonomy.${TARGET}-board.elf)
target_sources(gimbtonomy.${TARGET}-board.elf PRIVATE src/main.cpp)
target_link_libraries(gimbtonomy.${TARGET}-board.elf PRIVATE CANMsg Neopixel_Blocking)
target_set_firmware_properties(gimbtonomy.${TARGET}-board.elf)
