add_executable(gamepad.${TARGET}-board.elf)
target_sources(gamepad.${TARGET}-board.elf PRIVATE src/main.cpp)
target_link_libraries(gamepad.${TARGET}-board.elf PRIVATE AnalogInputGroup DigitalInputGroup FrameArbiter Logger)
target_set_firmware_properties(gamepad.${TARGET}-board.elf)
