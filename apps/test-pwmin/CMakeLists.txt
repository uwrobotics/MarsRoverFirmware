add_executable(test-pwmin.${TARGET}-board.elf)
target_sources(test-pwmin.${TARGET}-board.elf PRIVATE apps/test-pwmin/main.cpp)
target_set_firmware_properties(test-pwmin.${TARGET}-board.elf)
