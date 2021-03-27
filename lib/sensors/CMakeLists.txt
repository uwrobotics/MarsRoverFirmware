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

add_library(BQ40Z80 STATIC)
target_sources(BQ40Z80 PRIVATE src/BQ40Z80.cpp)
target_include_directories(BQ40Z80 PUBLIC include)
target_link_libraries(BQ40Z80 PRIVATE Sensor SMBus)
target_set_mbed_dependency(BQ40Z80)


