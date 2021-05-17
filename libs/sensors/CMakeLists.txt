add_library(Sensor INTERFACE)
target_include_directories(Sensor INTERFACE include)
target_link_libraries(Sensor
        INTERFACE
        mbed-os)

add_library(AdafruitSTEMMA STATIC)
target_sources(AdafruitSTEMMA PRIVATE src/AdafruitSTEMMA.cpp)
target_include_directories(AdafruitSTEMMA PUBLIC include)
target_link_libraries(AdafruitSTEMMA
        PRIVATE
        Sensor
        mbed-os
        )

add_library(CurrentSensor STATIC)
target_sources(CurrentSensor PRIVATE src/CurrentSensor.cpp)
target_include_directories(CurrentSensor PUBLIC include)
target_link_libraries(CurrentSensor
        PRIVATE
        Sensor
        mbed-os
        )
