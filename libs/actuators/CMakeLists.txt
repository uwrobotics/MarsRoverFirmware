add_library(Actuator INTERFACE)
target_include_directories(Actuator INTERFACE include)

add_library(DCMotor STATIC)
target_sources(DCMotor PRIVATE src/DCMotor.cpp)
target_include_directories(DCMotor PUBLIC include)
target_link_libraries(DCMotor
        PRIVATE
        Actuator
        mbed-os
        )

add_library(BrushlessMotor STATIC)
target_sources(BrushlessMotor PRIVATE src/BrushlessMotor.cpp)
target_include_directories(BrushlessMotor PUBLIC include)
target_link_libraries(BrushlessMotor
        PRIVATE
        Actuator
        mbed-os
        )

add_library(Servo INTERFACE)
target_include_directories(Servo INTERFACE include)

add_library(ContServo STATIC)
target_sources(ContServo PRIVATE src/ContServo.cpp)
target_include_directories(ContServo PUBLIC include)
target_link_libraries(ContServo
        PRIVATE
        Actuator
        Servo
        mbed-os
        )

add_library(LimServo STATIC)
target_sources(LimServo PRIVATE src/LimServo.cpp)
target_include_directories(LimServo PUBLIC include)
target_link_libraries(LimServo
        PRIVATE
        Actuator
        Servo
        mbed-os
        )
