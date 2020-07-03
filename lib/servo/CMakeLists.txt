add_library(Servo)
target_sources(Servo PRIVATE
        src/Servo.cpp)
target_include_directories(Servo PUBLIC include)
target_link_libraries(Servo PUBLIC mbed-os)
target_precompile_headers(Servo REUSE_FROM mbed-os)
