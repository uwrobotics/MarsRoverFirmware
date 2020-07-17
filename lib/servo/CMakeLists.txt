add_library(Servo INTERFACE)
target_include_directories(Servo INTERFACE include)

add_library(ContServo)
target_sources(ContServo PRIVATE src/ContServo.cpp)
target_include_directories(ContServo PUBLIC include)
target_set_mbed_dependency(ContServo)

add_library(LimServo)
target_sources(LimServo PRIVATE src/LimServo.cpp)
target_include_directories(LimServo PUBLIC include)
target_set_mbed_dependency(LimServo)
