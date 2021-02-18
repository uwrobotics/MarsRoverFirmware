add_library(Sensor INTERFACE)
target_include_directories(Sensor INTERFACE include)

add_library(AdafruitSTEMMA STATIC)
target_sources(AdafruitSTEMMA PRIVATE src/AdafruitSTEMMA.cpp)
target_include_directories(AdafruitSTEMMA PUBLIC include)
target_link_libraries(AdafruitSTEMMA PRIVATE Sensor)
target_set_mbed_dependency(AdafruitSTEMMA)

add_library(CurrentSensor STATIC)
target_sources(CurrentSensor PRIVATE src/CurrentSensor.cpp)
target_include_directories(CurrentSensor PUBLIC include)
target_link_libraries(CurrentSensor PRIVATE Sensor)
target_set_mbed_dependency(CurrentSensor)

