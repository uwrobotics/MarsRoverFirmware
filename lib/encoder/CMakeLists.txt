add_library(Encoder INTERFACE)
target_include_directories(Encoder INTERFACE include)

add_library(EncoderMAE3 STATIC)
target_sources(EncoderMAE3 PRIVATE src/MAE3.cpp)
target_include_directories(EncoderMAE3 PUBLIC include)
target_link_libraries(EncoderMAE3 PRIVATE PwmIn Encoder)
target_set_mbed_dependency(EncoderMAE3)

add_library(Quadrature64CPR STATIC)
target_sources(Quadrature64CPR PRIVATE src/Quadrature64CPR.cpp)
target_include_directories(Quadrature64CPR PUBLIC include)
target_link_libraries(Quadrature64CPR PRIVATE QEI Encoder)
target_set_mbed_dependency(Quadrature64CPR)


add_library(EncoderAEAT6012 STATIC)
target_sources(EncoderAEAT6012 PRIVATE src/AEAT6012.cpp)
target_include_directories(EncoderAEAT6012 PUBLIC include)
target_link_libraries(EncoderAEAT6012 PRIVATE Encoder)
target_set_mbed_dependency(EncoderAEAT6012)

add_library(EncoderNetzer STATIC)
target_sources(EncoderNetzer PRIVATE src/Netzer.cpp)
target_include_directories(EncoderNetzer PUBLIC include)
target_link_libraries(EncoderNetzer PRIVATE Encoder)
target_set_mbed_dependency(EncoderNetzer)

