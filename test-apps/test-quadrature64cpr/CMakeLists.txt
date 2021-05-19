add_executable(test-quadrature64cpr)
target_sources(test-quadrature64cpr PRIVATE src/main.cpp)
target_link_libraries(test-quadrature64cpr PRIVATE Quadrature64CPR QEI Logger)
mbed_set_post_build(test-quadrature64cpr)

