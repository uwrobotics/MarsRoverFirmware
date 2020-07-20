add_library(ActuatorController STATIC)
target_sources(ActuatorController PRIVATE src/ActuatorController.cpp)
target_include_directories(ActuatorController
        PUBLIC
        include
        PRIVATE
        ../sensors/include #TODO(wmmc88): Delete this hack.should seperate public and private headers and include properly. Encoder.h should be interface library?
        ../pid/include #TODO(wmmc88): Delete this hack. should seperate public and private headers and include properly.
        )
target_link_libraries(ActuatorController PRIVATE EncoderAbsolute_PWM PID Motor)
target_set_mbed_dependency(ActuatorController)

add_library(Motor STATIC)
target_sources(Motor PRIVATE src/Motor.cpp)
target_include_directories(Motor PUBLIC include)
target_set_mbed_dependency(Motor)

add_library(ServoMotor STATIC)
target_sources(ServoMotor PRIVATE src/ServoMotor.cpp)
target_include_directories(ServoMotor PUBLIC include)
target_link_libraries(ServoMotor PRIVATE ContServo Motor)
target_set_mbed_dependency(ServoMotor)
