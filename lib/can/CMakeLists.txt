add_library(CANBuffer STATIC)
target_sources(CANBuffer PRIVATE src/CANBuffer.cpp)
target_include_directories(CANBuffer PUBLIC include)
target_link_libraries(CANBuffer
        PRIVATE
        LUT
        uwrt-mars-rover-hw-bridge
        )
target_set_mbed_dependency(CANBuffer)

add_library(CANMsg INTERFACE)
target_include_directories(CANMsg INTERFACE include)
target_link_libraries(CANMsg
        INTERFACE
        LUT
        )

add_library(CANBus STATIC)
target_sources(CANBus PRIVATE src/CANBus.cpp)
target_include_directories(CANBus PUBLIC include)
target_link_libraries(CANBus
        PRIVATE
        uwrt-mars-rover-hw-bridge
        )
target_set_mbed_dependency(CANBus)
