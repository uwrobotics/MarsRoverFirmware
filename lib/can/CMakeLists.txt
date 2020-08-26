add_library(CANBuffer STATIC)
target_sources(CANBuffer PRIVATE src/CANBuffer.cpp)
target_include_directories(CANBuffer PUBLIC include)
target_link_libraries(CANBuffer
        PRIVATE
        LUT
        )
target_set_mbed_dependency(CANBuffer)

add_library(CANMsg INTERFACE)
target_include_directories(CANMsg INTERFACE include)
