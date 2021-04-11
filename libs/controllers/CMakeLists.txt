add_library(ActuatorControllerManager STATIC)
target_sources(ActuatorControllerManager PRIVATE src/ActuatorControllerManager.cpp)
target_include_directories(ActuatorControllerManager PUBLIC include)
target_link_libraries(ActuatorControllerManager
        PUBLIC
        ActuatorController
        PRIVATE
        LookupTable
        uwrt-mars-rover-hw-bridge
        mbed-os
        )

add_library(ActuatorController STATIC)
target_sources(ActuatorController PRIVATE src/ActuatorController.cpp)
target_include_directories(ActuatorController PUBLIC include)
target_link_libraries(ActuatorController
        PUBLIC
        Actuator
        CurrentSensor
        Encoder
        PID
        PRIVATE
        mbed-os
        )

add_library(PositionController STATIC)
target_sources(PositionController PRIVATE src/Position.cpp)
target_include_directories(PositionController PUBLIC include)
target_link_libraries(PositionController
        PUBLIC
        ActuatorController
        PRIVATE
        mbed-os
        )

add_library(VelocityController STATIC)
target_sources(VelocityController PRIVATE src/Velocity.cpp)
target_include_directories(VelocityController PUBLIC include)
target_link_libraries(VelocityController
        PRIVATE
        ActuatorController
        mbed-os
        )

add_library(CurrentController STATIC)
target_sources(CurrentController PRIVATE src/Current.cpp)
target_include_directories(CurrentController PUBLIC include)
target_link_libraries(CurrentController
        PRIVATE
        ActuatorController
        mbed-os
        )

add_library(OpenLoopController STATIC)
target_sources(OpenLoopController PRIVATE src/OpenLoop.cpp)
target_include_directories(OpenLoopController PUBLIC include)
target_link_libraries(OpenLoopController
        PRIVATE
        Actuator
        ActuatorController
        CurrentSensor
        Encoder
        mbed-os
        )
