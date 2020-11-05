add_library(ActuatorController STATIC)
target_sources(ActuatorController PRIVATE src/ActuatorController.cpp)
target_include_directories(ActuatorController PUBLIC include)
target_link_libraries(ActuatorController
        PUBLIC
        PID
        PRIVATE
        EncoderAbsolute_PWM
        DCMotor
        )
target_set_mbed_dependency(ActuatorController)
