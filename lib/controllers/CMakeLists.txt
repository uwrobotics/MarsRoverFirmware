add_library(ActuatorController STATIC)
target_sources(ActuatorController PRIVATE src/ActuatorController.cpp)
target_include_directories(ActuatorController PUBLIC include)
target_link_libraries(ActuatorController
        PUBLIC
        PID
        PRIVATE
        EncoderAbsolute_PWM
        Motor
        )
target_set_mbed_dependency(ActuatorController)

# add_library(Motor STATIC)
# target_sources(Motor PRIVATE src/Motor.cpp)
# target_include_directories(Motor PUBLIC include)
# target_set_mbed_dependency(Motor)

# TODO: Need to update dependencies to align with new actuators library (cindyli-13)