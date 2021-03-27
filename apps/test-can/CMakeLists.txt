add_executable(test-can.${TARGET}-board.elf)
target_sources(test-can.${TARGET}-board.elf PRIVATE src/main.cpp)
target_include_directories(test-can.${TARGET}-board.elf PUBLIC include)
target_link_libraries(test-can.${TARGET}-board.elf 
    PRIVATE 
    CANInterface 
    CANMsg     
    uwrt-mars-rover-hw-bridge
    Logger
)
target_set_firmware_properties(test-can.${TARGET}-board.elf)
