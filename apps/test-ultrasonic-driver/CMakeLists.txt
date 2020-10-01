add_executable(test-ultrasonic-driver.${TARGET}-board.elf)
target_sources(test-ultrasonic-driver.${TARGET}-board.elf PRIVATE src/main.cpp)
target_link_libraries(test-ultrasonic-driver.${TARGET}-board.elf PRIVATE UltraSonicSensor)
target_set_firmware_properties(test-ultrasonic-driver.${TARGET}-board.elf)