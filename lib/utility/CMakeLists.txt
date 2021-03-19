add_library(Logger STATIC)
target_sources(Logger PRIVATE src/Logger.cpp)
target_include_directories(Logger PUBLIC include)
target_set_mbed_dependency(Logger)

add_library(LookupTable INTERFACE)
target_include_directories(LookupTable INTERFACE include)

add_library(SMBus STATIC)
target_sources(SMBus PRIVATE src/SMBus.cpp)
target_include_directories(SMBus PUBLIC include)
target_set_mbed_dependency(SMBus)
