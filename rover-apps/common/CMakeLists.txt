add_library(WatchdogModule STATIC)
target_sources(WatchdogModule PRIVATE src/WatchdogModule.cpp)
target_include_directories(WatchdogModule PUBLIC include)
target_link_libraries(WatchdogModule
        PRIVATE
        WatchdogWrapper
        mbed-os
        )