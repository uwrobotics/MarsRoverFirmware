add_executable(tutorial-servo-can-control)
target_sources(tutorial-servo-can-control PRIVATE src/main.cpp)
# tells cmake that the main.cpp depends on these libraries
target_link_libraries (tutorial-servo-can-control 
PRIVATE
TutorialServo
mbed-os
CANMsg)
mbed_set_post_build(tutorial-servo-can-control)

#declare the TutorialServo library
add_library (TutorialServo STATIC)
target_sources (TutorialServo PRIVATE src/TutorialServo.cpp)
target_include_directories (TutorialServo PUBLIC include)
target_link_libraries (TutorialServo PRIVATE mbed-os)
