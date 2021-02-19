add_library(Logger STATIC)
target_sources(Logger PRIVATE src/Logger.cpp)
target_include_directories(Logger PUBLIC include)
target_set_mbed_dependency(Logger)

add_library(LookupTable INTERFACE)
target_include_directories(LookupTable INTERFACE include)
