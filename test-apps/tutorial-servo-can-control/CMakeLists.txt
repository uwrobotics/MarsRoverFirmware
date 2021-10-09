add_executable(tutorial-servo-can-control)
target_sources(tutorial-servo-can-control PRIVATE src/main.cpp)
target_link_libraries(tutorial-servo-can-control PRIVATE mbed-os)
mbed_set_post_build(tutorial-servo-can-control)

add_library (TutorialServo STATIC)
target_sources (TutorialServo PRIVATE src/TutorialServo.cpp)
target_link_libraries (tutorial-servo-can-control PRIVATE
TutorialServo)
target_include_directories (TutorialServo PUBLIC include)
target_link_libraries (TutorialServo PRIVATE mbed-os)
target_link_libraries (tutorial-servo-can-control PRIVATE TutorialServo CANMsg)
