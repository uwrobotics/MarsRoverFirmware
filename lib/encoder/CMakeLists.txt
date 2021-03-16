add_library(Encoder INTERFACE)
target_include_directories(Encoder INTERFACE include)

add_library(EncoderMAE3 STATIC)
target_sources(EncoderMAE3 PRIVATE src/MAE3.cpp)
target_include_directories(EncoderMAE3 PUBLIC include)
target_link_libraries(EncoderMAE3 PRIVATE PwmIn Encoder)
target_set_mbed_dependency(EncoderMAE3)

add_library(EncoderPololu37D STATIC)
target_sources(EncoderPololu37D PRIVATE src/Pololu37D.cpp)
target_include_directories(EncoderPololu37D PUBLIC include)
target_link_libraries(EncoderPololu37D PRIVATE QEI Encoder)
target_set_mbed_dependency(EncoderPololu37D)


add_library(EncoderAEAT6012 STATIC)
target_sources(EncoderAEAT6012 PRIVATE src/AEAT6012.cpp)
target_include_directories(EncoderAEAT6012 PUBLIC include)
target_link_libraries(EncoderAEAT6012 PRIVATE Encoder)
target_set_mbed_dependency(EncoderAEAT6012)
