add_library(ActuatorController OBJECT)
target_sources(ActuatorController PRIVATE
        src/ActuatorController.cpp)
target_include_directories(ActuatorController
        PUBLIC
        include
        PRIVATE
        ../sensors/include #TODO(wmmc88): Delete this hack.should seperate public and private headers and include properly. Encoder.h should be interface library?
        ../pid/include #TODO(wmmc88): Delete this hack. should seperate public and private headers and include properly.
        )
target_link_libraries(ActuatorController PUBLIC mbed-os EncoderAbsolute_PWM)
target_precompile_headers(ActuatorController REUSE_FROM mbed-os)

add_library(Motor OBJECT)
target_sources(Motor PRIVATE
        src/Motor.cpp)
target_include_directories(Motor PUBLIC include)
target_link_libraries(Motor PUBLIC mbed-os)
target_precompile_headers(Motor REUSE_FROM mbed-os)

add_library(ServoMotor OBJECT)
target_sources(ServoMotor PRIVATE
        src/ServoMotor.cpp)
target_include_directories(ServoMotor PUBLIC include)
target_link_libraries(ServoMotor PUBLIC mbed-os)
target_precompile_headers(ServoMotor REUSE_FROM mbed-os)

