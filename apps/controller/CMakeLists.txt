add_executable(controller.${TARGET}-board.elf)
target_sources(controller.${TARGET}-board.elf PRIVATE src/main.cpp)
#target_link_libraries(arm.${TARGET}-board.elf PRIVATE )
target_set_firmware_properties(controller.${TARGET}-board.elf)
