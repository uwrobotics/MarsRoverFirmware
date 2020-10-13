add_executable(tutorial-servo-can-control.${TARGET}-board.elf)
target_sources(tutorial-servo-can-control.${TARGET}-board.elf PRIVATE src/main.cpp)
target_sources(tutorial-servo-can-control.${TARGET}-board.elf 
    PRIVATE 
    TutorialServo
)
target_set_firmware_properties(tutorial-servo-can-control.${TARGET}-board.elf)

add_library(TutorialServo STATIC)
target_sources(TutorialServo PRIVATE src/TutorialServo.cpp)
target_include_directories(TutorialServo PUBLIC include)
target_set_mbed_dependency(TutorialServo)