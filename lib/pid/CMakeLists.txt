add_library(PID OBJECT)
target_sources(PID PRIVATE
        src/PID.cpp)
target_include_directories(PID PUBLIC include)
target_link_libraries(PID PUBLIC mbed-os)
target_precompile_headers(PID REUSE_FROM mbed-os)
