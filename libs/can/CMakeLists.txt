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

add_library(CANSignal INTERFACE)
target_include_directories(CANMsg INTERFACE include)
target_link_libraries(CANSignal
        INTERFACE
        uwrt-mars-rover-hw-bridge
        LookupTable
        )

add_library(CANAutogenWrapper STATIC)
target_sources(CANAutogenWrapper PRIVATE src/CANAutogenWrapper.cpp)
target_include_directories(CANAutogenWrapper PUBLIC include)
target_link_libraries(CANAutogenWrapper
        PRIVATE
        uwrt-mars-rover-hw-bridge
        CANSignal
        )
target_set_mbed_dependency(CANAutogenWrapper)

add_library(CANBus STATIC)
target_sources(CANBus PRIVATE src/CANBus.cpp)
target_include_directories(CANBus PUBLIC include)
target_link_libraries(CANBus
        PUBLIC
        mbed-os
        PRIVATE
        uwrt-mars-rover-hw-bridge
        CANMsg
        CANSignal
        CANAutogenWrapper
        )
