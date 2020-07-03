add_library(CANBuffer)
target_sources(CANBuffer PRIVATE src/CANBuffer.cpp)
target_include_directories(CANBuffer PUBLIC include)
target_set_mbed_dependency(CANBuffer)

add_library(CANMsg INTERFACE)
target_include_directories(CANMsg INTERFACE include)
