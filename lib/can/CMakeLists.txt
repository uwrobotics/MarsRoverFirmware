add_library(CANBuffer)
target_sources(CANBuffer PRIVATE
        src/CANBuffer.cpp)
target_include_directories(CANBuffer PUBLIC include)
target_set_mbed_dependency(CANBuffer)
