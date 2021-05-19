add_library(Encoder INTERFACE)
target_include_directories(Encoder INTERFACE include)

add_library(EncoderMAE3 STATIC)
target_sources(EncoderMAE3 PRIVATE src/MAE3.cpp)
target_include_directories(EncoderMAE3 PUBLIC include)
target_link_libraries(EncoderMAE3
        PRIVATE
        PwmIn
        Encoder
        mbed-os
        )

add_library(Quadrature64CPR STATIC)
target_sources(Quadrature64CPR PRIVATE src/Quadrature64CPR.cpp)
target_include_directories(Quadrature64CPR PUBLIC include)
target_link_libraries(Quadrature64CPR
        PRIVATE
        QEI
        Encoder
        mbed-os
        )


add_library(EncoderAEAT6012 STATIC)
target_sources(EncoderAEAT6012 PRIVATE src/AEAT6012.cpp)
target_include_directories(EncoderAEAT6012 PUBLIC include)
target_link_libraries(EncoderAEAT6012
        PUBLIC
        mbed-os
        PRIVATE
        Encoder
        )
