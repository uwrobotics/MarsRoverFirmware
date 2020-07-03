add_library(Servo)
target_sources(Servo PRIVATE
        src/Servo.cpp)
target_include_directories(Servo PUBLIC include)
target_set_mbed_dependency(Servo)
