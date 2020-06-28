add_library(EncoderAbsolute_PWM OBJECT)
target_sources(EncoderAbsolute_PWM PRIVATE
        src/EncoderAbsolute_PWM.cpp)
target_include_directories(EncoderAbsolute_PWM PUBLIC include)
target_link_libraries(EncoderAbsolute_PWM PUBLIC mbed-os)
target_precompile_headers(EncoderAbsolute_PWM REUSE_FROM mbed-os)

add_library(EncoderRelative_Quadrature OBJECT)
target_sources(EncoderRelative_Quadrature PRIVATE
        src/EncoderRelative_Quadrature.cpp)
target_include_directories(EncoderRelative_Quadrature PUBLIC include)
target_link_libraries(EncoderRelative_Quadrature PUBLIC mbed-os)
target_precompile_headers(EncoderRelative_Quadrature REUSE_FROM mbed-os)

add_library(MoistureSensor OBJECT)
target_sources(MoistureSensor PRIVATE
        src/MoistureSensor.cpp)
target_include_directories(MoistureSensor PUBLIC include)
target_link_libraries(MoistureSensor PUBLIC mbed-os)
target_precompile_headers(MoistureSensor REUSE_FROM mbed-os)

add_library(PwmIn OBJECT)
target_sources(PwmIn PRIVATE
        src/PwmIn.cpp)
target_include_directories(PwmIn PUBLIC include)
target_link_libraries(PwmIn PUBLIC mbed-os)
target_precompile_headers(PwmIn REUSE_FROM mbed-os)

add_library(QEI OBJECT)
target_sources(QEI PRIVATE
        src/QEI.cpp)
target_include_directories(QEI PUBLIC include)
target_link_libraries(QEI PUBLIC mbed-os)
target_precompile_headers(QEI REUSE_FROM mbed-os)
