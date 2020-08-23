add_library(LUT STATIC)
target_sources(LUT PRIVATE src/LUT.cpp)
target_include_directories(LUT PUBLIC include)
target_set_mbed_dependency(LUT)
