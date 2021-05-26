add_library(CANMsg INTERFACE)
target_include_directories(CANMsg INTERFACE include)
target_link_libraries(CANMsg
        INTERFACE
        uwrt-mars-rover-hw-bridge
        LookupTable
        mbed-os
        )

add_library(CANBuffer STATIC)
target_sources(CANBuffer PRIVATE src/CANBuffer.cpp)
target_include_directories(CANBuffer PUBLIC include)
target_link_libraries(CANBuffer
        PRIVATE
        uwrt-mars-rover-hw-bridge
        LookupTable
        mbed-os
        )

add_library(CANBus STATIC)
target_sources(CANBus PRIVATE src/CANBus.cpp)
target_include_directories(CANBus PUBLIC include)
target_link_libraries(CANBus
        PRIVATE
        uwrt-mars-rover-hw-bridge
        mbed-os
        )

add_library(CANInterface STATIC)
target_sources(CANInterface PRIVATE src/CANInterface.cpp)
target_include_directories(CANInterface PUBLIC include)
target_link_libraries(CANInterface
        PRIVATE
        uwrt-mars-rover-hw-bridge
        CANBus
        CANMsg
        mbed-os
        mbed-events
        )
