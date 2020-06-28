add_library(Neopixel_Blocking OBJECT)
target_sources(Neopixel_Blocking PRIVATE
        src/Neopixel_Blocking.cpp)
target_include_directories(Neopixel_Blocking PUBLIC include)
target_link_libraries(Neopixel_Blocking PUBLIC mbed-os)
target_precompile_headers(Neopixel_Blocking REUSE_FROM mbed-os)
