add_library (TutorialServo STATIC)
target_sources (TutorialServo PRIVATE src/TutorialServo.cpp)
target_link_libraries (tutorial-servo-can-control. ${TARGET} -board.elf
PRIVATE
TutorialServo
)
target_link_libraries (tutorial-servo-pot-control. ${TARGET} -board.elf
PRIVATE
TutorialServo
)
target_include_directories (TutorialServo PUBLIC include)
target_set_mbed_dependency (TutorialServo)