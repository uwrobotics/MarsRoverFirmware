add_library(AnalogBusIn STATIC)
target_sources(AnalogBusIn PRIVATE src/AnalogBusIn.cpp)
target_include_directories(AnalogBusIn PUBLIC include)
target_link_libraries(AnalogBusIn
        PUBLIC
        mbed-os
        )

add_library(AnalogInputGroup STATIC)
target_sources(AnalogInputGroup PRIVATE src/AnalogInputGroup.cpp)
target_include_directories(AnalogInputGroup PUBLIC include)
target_link_libraries(AnalogInputGroup
        PRIVATE
        AnalogBusIn
        mbed-os
        )

add_library(DigitalInputGroup STATIC)
target_sources(DigitalInputGroup PRIVATE src/DigitalInputGroup.cpp)
target_include_directories(DigitalInputGroup PUBLIC include)
target_link_libraries(DigitalInputGroup
        PRIVATE
        AnalogBusIn
        mbed-os
        )

add_library(FrameArbiter STATIC)
target_sources(FrameArbiter PRIVATE src/FrameArbiter.cpp)
target_include_directories(FrameArbiter PUBLIC include)
target_link_libraries(FrameArbiter
        PRIVATE
        AnalogInputGroup
        DigitalInputGroup
        mbed-os
        )

