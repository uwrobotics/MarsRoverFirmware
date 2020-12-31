add_executable(tutorial-servo-pot-control.${TARGET}-board.elf)
target_sources(tutorial-servo-pot-control.${TARGET}-board.elf PRIVATEsrc/main.cpp)
target_set_firmware_properties(tutorial-servo-pot-control.${TARGET}-board.elf)
