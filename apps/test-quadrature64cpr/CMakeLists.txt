add_executable(test-quadrature64cpr.${TARGET}-board.elf)
target_sources(test-quadrature64cpr.${TARGET}-board.elf PRIVATE src/main.cpp)
target_link_libraries(test-quadrature64cpr.${TARGET}-board.elf PRIVATE Quadrature64CPR QEI Logger)
target_set_firmware_properties(test-quadrature64cpr.${TARGET}-board.elf)
