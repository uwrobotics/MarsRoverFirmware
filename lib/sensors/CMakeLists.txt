add_library(Sensor INTERFACE)
target_include_directories(Sensor INTERFACE include)

add_library(MoistureSensor STATIC)
target_sources(MoistureSensor PRIVATE src/MoistureSensor.cpp)
target_include_directories(MoistureSensor PUBLIC include)
target_link_libraries(MoistureSensor PRIVATE Sensor)
target_set_mbed_dependency(MoistureSensor)

