add_executable(test-stress-can.${TARGET}-board.elf)
target_sources(test-stress-can.${TARGET}-board.elf PRIVATE src/main.cpp)
target_include_directories(test-stress-can.${TARGET}-board.elf PUBLIC include)
target_link_libraries(test-stress-can.${TARGET}-board.elf 
    PRIVATE 
    CANInterface 
    CANMsg     
    uwrt-mars-rover-hw-bridge
    Logger
)
target_set_firmware_properties(test-stress-can.${TARGET}-board.elf)
