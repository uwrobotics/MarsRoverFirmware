add_library(PID STATIC)
target_sources(PID PRIVATE src/PID.cpp)
target_include_directories(PID PUBLIC include)
target_set_mbed_dependency(PID)
