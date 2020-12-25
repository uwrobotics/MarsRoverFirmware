add_executable(test-pid.${TARGET}-board.elf)
target_sources(test-pid.${TARGET}-board.elf PRIVATE src/main.cpp)
target_link_libraries(test-pid.${TARGET}-board.elf PRIVATE PID SimulatedPIDValues)
target_set_firmware_properties(test-pid.${TARGET}-board.elf)

add_library(SimulatedPIDValues INTERFACE)
target_include_directories(SimulatedPIDValues INTERFACE include)
