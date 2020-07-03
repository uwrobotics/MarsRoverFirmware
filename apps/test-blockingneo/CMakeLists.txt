add_executable(test-blockingneo.${TARGET}-board.elf)
target_sources(test-blockingneo.${TARGET}-board.elf PRIVATE apps/test-blockingneo/main.cpp)
target_set_firmware_properties(test-blockingneo.${TARGET}-board.elf)
