add_library(PwmIn STATIC)
target_sources(PwmIn PRIVATE src/PwmIn.cpp)
target_include_directories(PwmIn PUBLIC include)
target_set_mbed_dependency(PwmIn)

add_library(QEI STATIC)
target_sources(QEI PRIVATE src/QEI.cpp)
target_include_directories(QEI PUBLIC include)
target_set_mbed_dependency(QEI)
