add_library(Logger STATIC)
target_sources(Logger PRIVATE src/Logger.cpp)
target_include_directories(Logger PUBLIC include)
target_link_libraries(Logger PUBLIC mbed-os)

add_library(WatchdogWrapper STATIC)
target_sources(WatchdogWrapper PRIVATE src/WatchdogWrapper.cpp)
target_include_directories(WatchdogWrapper PUBLIC include)
target_link_libraries(WatchdogWrapper PUBLIC Logger mbed-os)

add_library(LookupTable INTERFACE)
target_include_directories(LookupTable INTERFACE include)

add_library(Units INTERFACE)
target_include_directories(Units INTERFACE include)
