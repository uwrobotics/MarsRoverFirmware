add_library(AdafruitSTEMMAModule)
target_sources(AdafruitSTEMMAModule PRIVATE src/AdafruitSTEMMAModule.cpp)
target_include_directories(AdafruitSTEMMAModule PRIVATE include)
target_link_libraries(AdafruitSTEMMAModule
        PRIVATE
        mbed-os        
        PUBLIC
        AdafruitSTEMMA 
        Module
)

add_executable(science)
target_sources(science PRIVATE ../common/src/main.cpp)
target_include_directories(science PUBLIC include ../common/include)
target_link_libraries(science
        PRIVATE
        #Control
        OpenLoopController
        PositionController
        VelocityController
        CurrentController
        ActuatorControllerManager
        #Motors
        LimServo
        DCMotor
        #Encoders
        EncoderAEAT6012
        EncoderMAE3
        Quadrature64CPR
        #CAN
        CANInterface
        CANMsg
        #Sensor
        CurrentSensor
        AdafruitSTEMMA
        #common-modules
        AdafruitSTEMMAModule
        #Other
        uwrt-mars-rover-hw-bridge
        Logger
        mbed-events
        )
mbed_set_post_build(science)

add_library(ScienceConfig INTERFACE)
target_include_directories(ScienceConfig INTERFACE include)
