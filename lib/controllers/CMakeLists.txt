add_library(ActuatorController)
target_sources(ActuatorController PRIVATE src/ActuatorController.cpp)
target_include_directories(ActuatorController
        PUBLIC
        include
        PRIVATE
        ../sensors/include #TODO(wmmc88): Delete this hack.should seperate public and private headers and include properly. Encoder.h should be interface library?
        ../pid/include #TODO(wmmc88): Delete this hack. should seperate public and private headers and include properly.
        )
target_link_libraries(ActuatorController PUBLIC EncoderAbsolute_PWM PID Motor)
target_set_mbed_dependency(ActuatorController)

add_library(Motor)
target_sources(Motor PRIVATE src/Motor.cpp)
target_include_directories(Motor PUBLIC include)
target_set_mbed_dependency(Motor)

#add_library(ServoMotor) #TODO(qinyang-bao): fix all compile errors and renable servo code
#target_sources(ServoMotor PRIVATE src/ .cpp)
#target_include_directories(ServoMotor PUBLIC include)
#target_set_mbed_dependency(ServoMotor)
