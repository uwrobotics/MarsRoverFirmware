target_include_directories(Encoder INTERFACE include)

add_library(PwmIn STATIC)
target_sources(PwmIn PRIVATE src/PwmIn.cpp)
target_include_directories(PwmIn PUBLIC include)
target_link_libraries(PwmIn
        PRIVATE
        mbed-os
        )

add_library(QEI STATIC)
target_sources(QEI PRIVATE src/QEI.cpp)
target_include_directories(QEI PUBLIC include)
target_include_directories(QEI PUBLIC Encoder)
target_link_libraries(QEI
        PRIVATE
        mbed-os
        )
