add_library(Encoder INTERFACE)
target_include_directories(Encoder INTERFACE include)

add_library(EncoderAbsolute_PWM STATIC)
target_sources(EncoderAbsolute_PWM PRIVATE src/EncoderAbsolute_PWM.cpp)
target_include_directories(EncoderAbsolute_PWM PUBLIC include)
target_link_libraries(EncoderAbsolute_PWM PRIVATE PwmIn)
target_set_mbed_dependency(EncoderAbsolute_PWM)

add_library(EncoderRelative_Quadrature STATIC)
target_sources(EncoderRelative_Quadrature PRIVATE src/EncoderRelative_Quadrature.cpp)
target_include_directories(EncoderRelative_Quadrature PUBLIC include)
target_link_libraries(EncoderRelative_Quadrature PRIVATE QEI)
target_set_mbed_dependency(EncoderRelative_Quadrature)

add_library(MoistureSensor STATIC)
target_sources(MoistureSensor PRIVATE src/MoistureSensor.cpp)
target_include_directories(MoistureSensor PUBLIC include)
target_set_mbed_dependency(MoistureSensor)

add_library(PwmIn STATIC)
target_sources(PwmIn PRIVATE src/PwmIn.cpp)
target_include_directories(PwmIn PUBLIC include)
target_set_mbed_dependency(PwmIn)

add_library(QEI STATIC)
target_sources(QEI PRIVATE src/QEI.cpp)
target_include_directories(QEI PUBLIC include)
target_set_mbed_dependency(QEI)
