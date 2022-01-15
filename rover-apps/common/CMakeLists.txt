add_library(WatchdogModule STATIC)
target_sources(WatchdogModule PRIVATE src/WatchdogModule.cpp)
target_include_directories(WatchdogModule PUBLIC include)
target_link_libraries(WatchdogModule
        PRIVATE
        WatchdogWrapper
        mbed-os
        )

add_library(CANModule)
target_sources(CANModule PRIVATE src/CANModule.cpp)
target_include_directories(CANModule PUBLIC include)
target_link_libraries(CANModule
        PRIVATE
        CANInterface
        mbed-os    
        )

