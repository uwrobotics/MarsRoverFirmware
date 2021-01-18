add_executable(test-urm04.${TARGET}-board.elf)
target_sources(test-urm04.${TARGET}-board.elf PRIVATE src/main.cpp)
target_link_libraries(test-urm04.${TARGET}-board.elf PRIVATE URM04Sensor uwrt-mars-rover-hw-bridge)
target_set_firmware_properties(test-urm04.${TARGET}-board.elf)