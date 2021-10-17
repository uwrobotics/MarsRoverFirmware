add_executable(tutorial-servo-pot-control)
target_sources(tutorial-servo-pot-control PRIVATE src/main.cpp)
target_link_libraries(tutorial-servo-pot-control PRIVATE mbed-os)
mbed_set_post_build(tutorial-servo-pot-control)