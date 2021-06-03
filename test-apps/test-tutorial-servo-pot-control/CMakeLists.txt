add_executable(test-tutorial-servo-pot-control.${TARGET}-board.elf)
target_sources(test-tutorial-servo-pot-control.${TARGET}-board.elf PRIVATE src/main.cpp)
target_set_firmware_properties(test-tutorial-servo-pot-control.${TARGET}-board.elf)
