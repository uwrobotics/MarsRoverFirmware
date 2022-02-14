add_library(WatchdogModule STATIC)
target_sources(WatchdogModule PRIVATE src/WatchdogModule.cpp)
target_include_directories(WatchdogModule PUBLIC include)
target_link_libraries(WatchdogModule
        PRIVATE
        WatchdogWrapper
        mbed-os
        )

add_library(CANDriverModule STATIC)
target_sources(CANDriverModule PRIVATE src/CANDriverModule.cpp)
target_include_directories(CANDriverModule PUBLIC include)
target_link_libraries(CANDriverModule
        PRIVATE
        CANInterface
        CANMsg
        mbed-os
        )

