add_library(SWO STATIC)
target_sources(SWO PRIVATE src/SWO.cpp)
target_include_directories(SWO PUBLIC include)
target_set_mbed_dependency(SWO)
