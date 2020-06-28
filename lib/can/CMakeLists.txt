add_library(CANBuffer OBJECT)
target_sources(CANBuffer PRIVATE
        src/CANBuffer.cpp)
target_include_directories(CANBuffer PUBLIC include)
target_link_libraries(CANBuffer PUBLIC mbed-os)
target_precompile_headers(CANBuffer REUSE_FROM mbed-os)
