add_executable(tutorial-servo-can-control.${TARGET}-board.elf)
target_sources(tutorial-servo-can-control.${TARGET}-board.elf PRIVATE src/main.cpp)
target_set_firmware_properties(tutorial-servo-can-control.${TARGET}-board.elf)
